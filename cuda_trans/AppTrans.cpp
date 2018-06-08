/*
* Copyright 2017-2018 NVIDIA Corporation.  All rights reserved.
*
* Please refer to the NVIDIA end user license agreement (EULA) associated
* with this source code for terms and conditions that govern your use of
* this software. Any use, reproduction, disclosure, or distribution of
* this software and related documentation outside the terms of the EULA
* is strictly prohibited.
*
*/

#include <cuda.h>
#include <cuda_runtime.h>
#include <iostream>
#include <memory>
#include <functional>
//#include <opencv2/opencv.hpp>
#include <cuda_nvcodec/NvEncoderCuda.h>
#include <cuda_nvcodec/NvDecoder.h>
#include <cuda_utils/NvCodecUtils.h>
#include <cuda_utils/NvEncoderCLIOptions.h>
#include <cuda_utils/FFmpegDemuxer.h>

simplelogger::Logger *logger = simplelogger::LoggerFactory::CreateConsoleLogger();

void ConvertToPlanar(uint8_t *pHostFrame, int nWidth, int nHeight, int nBitDepth) {
    if (nBitDepth == 8) {
        // nv12->iyuv
        YuvConverter<uint8_t> converter8(nWidth, nHeight);
        converter8.UVInterleavedToPlanar(pHostFrame);
    } else {
        // p016->yuv420p16
        YuvConverter<uint16_t> converter16(nWidth, nHeight);
        converter16.UVInterleavedToPlanar((uint16_t *)pHostFrame);
    }
}

void ShowHelpAndExit(const char *szBadOption = NULL)
{
    bool bThrowError = false;
    std::ostringstream oss;
    if (szBadOption) 
    {
        oss << "Error parsing \"" << szBadOption << "\"" << std::endl;
        bThrowError = true;
    }
    oss << "Options:" << std::endl
        << "-i           input_file" << std::endl
        << "-o           output_file" << std::endl
        << "-ob          Bit depth of the output: 8 10" << std::endl
        << "-gpu         Ordinal of GPU to use" << std::endl
        ;
    oss << NvEncoderInitParam().GetHelpMessage(false, false, true);
    if (bThrowError)
    {
        throw std::invalid_argument(oss.str());
    }
    else
    {
        std::cout << oss.str();
        exit(0);
    }
}

void ParseCommandLine(int argc, char *argv[], char *szInputFileName, char *szOutputFileName, int &nOutBitDepth, int &iGpu, NvEncoderInitParam &initParam) 
{
    std::ostringstream oss;
    int i;
    for (i = 1; i < argc; i++)
    {
        if (!_stricmp(argv[i], "-h"))
        {
            ShowHelpAndExit();
        }
        if (!_stricmp(argv[i], "-i"))
        {
            if (++i == argc)
            {
                ShowHelpAndExit("-i");
            }
            sprintf(szInputFileName, "%s", argv[i]);
            continue;
        }
        if (!_stricmp(argv[i], "-o"))
        {
            if (++i == argc)
            {
                ShowHelpAndExit("-o");
            }
            sprintf(szOutputFileName, "%s", argv[i]);
            continue;
        }
        if (!_stricmp(argv[i], "-ob"))
        {
            if (++i == argc)
            {
                ShowHelpAndExit("-ob");
            }
            nOutBitDepth = atoi(argv[i]);
            if (nOutBitDepth != 8 && nOutBitDepth != 10) 
            {
                ShowHelpAndExit("-ob");
            }
            continue;
        }
        if (!_stricmp(argv[i], "-gpu")) 
        {
            if (++i == argc)
            {
                ShowHelpAndExit("-gpu");
            }
            iGpu = atoi(argv[i]);
            continue;
        }
        // Regard as encoder parameter
        if (argv[i][0] != '-') 
        {
            ShowHelpAndExit(argv[i]);
        }
        oss << argv[i] << " ";
        while (i + 1 < argc && argv[i + 1][0] != '-') 
        {
            oss << argv[++i] << " ";
        }
    }
    initParam = NvEncoderInitParam(oss.str().c_str());
}

void Bitmap2Yuv420p_calc2(uint8_t *destination, uint8_t *rgb, size_t width, size_t height)
{
    size_t image_size = width * height;
    size_t upos = image_size;
    size_t vpos = upos + upos / 4;
    size_t i = 0;

    for( size_t line = 0; line < height; ++line )
    {
        if( !(line % 2) )
        {
            for( size_t x = 0; x < width; x += 2 )
            {
                uint8_t r = rgb[3 * i];
                uint8_t g = rgb[3 * i + 1];
                uint8_t b = rgb[3 * i + 2];

                destination[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;

                destination[upos++] = ((-38*r + -74*g + 112*b) >> 8) + 128;
                destination[vpos++] = ((112*r + -94*g + -18*b) >> 8) + 128;

                r = rgb[3 * i];
                g = rgb[3 * i + 1];
                b = rgb[3 * i + 2];

                destination[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;
            }
        }
        else
        {
            for( size_t x = 0; x < width; x += 1 )
            {
                uint8_t r = rgb[3 * i];
                uint8_t g = rgb[3 * i + 1];
                uint8_t b = rgb[3 * i + 2];

                destination[i++] = ((66*r + 129*g + 25*b) >> 8) + 16;
            }
        }
    }
}

int main(int argc, char **argv) {
    char szInFilePath[260] = "";
    char szOutFilePath[260] = "";
    int nOutBitDepth = 0;
    int iGpu = 0;
    CUdeviceptr pTmpImage = 0;

    try
    {
        using NvEncCudaPtr = std::unique_ptr<NvEncoderCuda, std::function<void(NvEncoderCuda*)>>;
        auto EncodeDeleteFunc = [](NvEncoderCuda *pEnc)
        {
            if (pEnc)
            {
                pEnc->DestroyEncoder();
                delete pEnc;
            }
        };
        NvEncCudaPtr pEnc(nullptr, EncodeDeleteFunc);

        NvEncoderInitParam encodeCLIOptions;
        ParseCommandLine(argc, argv, szInFilePath, szOutFilePath, nOutBitDepth, iGpu, encodeCLIOptions);

        CheckInputFile(szInFilePath);

        if (!*szOutFilePath) {
            sprintf(szOutFilePath, encodeCLIOptions.IsCodecH264() ? "out.h264" : "out.hevc");
        }

        std::ifstream fpIn(szInFilePath, std::ifstream::in | std::ifstream::binary);
        if (!fpIn)
        {
            std::ostringstream err;
            err << "Unable to open input file: " << szInFilePath << std::endl;
            throw std::invalid_argument(err.str());
        }

        std::ofstream fpOut(szOutFilePath, std::ios::out | std::ios::binary);
        if (!fpOut)
        {
            std::ostringstream err;
            err << "Unable to open output file: " << szOutFilePath << std::endl;
            throw std::invalid_argument(err.str());
        }

        ck(cuInit(0));
        int nGpu = 0;
        ck(cuDeviceGetCount(&nGpu));
        if (iGpu < 0 || iGpu >= nGpu) {
            std::cout << "GPU ordinal out of range. Should be within [" << 0 << ", " << nGpu - 1 << "]" << std::endl;
            return 1;
        }
        CUdevice cuDevice = 0;
        ck(cuDeviceGet(&cuDevice, iGpu));
        char szDeviceName[80];
        ck(cuDeviceGetName(szDeviceName, sizeof(szDeviceName), cuDevice));
        std::cout << "GPU in use: " << szDeviceName << std::endl;
        CUcontext cuContext = NULL;
        ck(cuCtxCreate(&cuContext, 0, cuDevice));

        // Output device frame
        FFmpegDemuxer demuxer(szInFilePath);
        NvDecoder dec(cuContext, demuxer.GetWidth(), demuxer.GetHeight(), true, FFmpeg2NvCodecId(demuxer.GetVideoCodec()), nullptr, false, true);
        int nWidth = demuxer.GetWidth(), nHeight = demuxer.GetHeight();
        int nFrameSize = nWidth * nHeight * 4;
        std::unique_ptr<uint8_t[]> pImage(new uint8_t[nFrameSize]);

        int nVideoBytes = 0, nFrameReturned = 0, nFrame = 0;
        uint8_t *pVideo = NULL, **ppFrame = NULL;
        bool bOut10 = false;
        CUdeviceptr dpFrame = 0;

        cuMemAlloc(&pTmpImage, nFrameSize);

        /*
        cv::Mat cv_overlay_img_rgba = cv::imread("overlay1.png", CV_LOAD_IMAGE_COLOR);
        cv::cvtColor(cv_overlay_img_rgba, cv_overlay_img_rgba, CV_BGR2RGBA);
        int overlay_width = cv_overlay_img_rgba.size().width;
        int overlay_height = cv_overlay_img_rgba.size().height;
        int overlay_pixels = overlay_width * overlay_height;
        uchar4 *h_overlay_img_rgba = (uchar4 *)cv_overlay_img_rgba.ptr<unsigned char>(0);

        uchar4 *d_overlay_img_rgba;
        // Allocate GPU memory for rgba and grey images
        cudaMalloc((void **)&d_overlay_img_rgba, overlay_pixels * sizeof(uchar4));
        cudaMemcpy(d_overlay_img_rgba, h_overlay_img_rgba, overlay_pixels * sizeof(uchar4),
                   cudaMemcpyHostToDevice);*/

        do {
            demuxer.Demux(&pVideo, &nVideoBytes);
            dec.Decode(pVideo, nVideoBytes, &ppFrame, &nFrameReturned);

            for (int i = 0; i < nFrameReturned; i++)
            {
                if (!pEnc)
                {
                    bOut10 = nOutBitDepth ? nOutBitDepth > 8 : dec.GetBitDepth() > 8;
                    pEnc.reset(new NvEncoderCuda(cuContext, dec.GetWidth(), dec.GetHeight(),
                        bOut10 ? NV_ENC_BUFFER_FORMAT_YUV420_10BIT : NV_ENC_BUFFER_FORMAT_NV12));

                    NV_ENC_INITIALIZE_PARAMS initializeParams = { NV_ENC_INITIALIZE_PARAMS_VER };
                    NV_ENC_CONFIG encodeConfig = { NV_ENC_CONFIG_VER };
                    initializeParams.encodeConfig = &encodeConfig;
                    pEnc->CreateDefaultEncoderParams(&initializeParams, encodeCLIOptions.GetEncodeGUID(), encodeCLIOptions.GetPresetGUID());

                    encodeCLIOptions.SetInitParams(&initializeParams, bOut10 ? NV_ENC_BUFFER_FORMAT_YUV420_10BIT : NV_ENC_BUFFER_FORMAT_NV12);

                    pEnc->CreateEncoder(&initializeParams);
                }

//                cudaNV12ToRGBA(ppFrame[i], (uchar4 *)pTmpImage, dec.GetWidth(), dec.GetHeight() );
                Nv12ToBgra32((uint8_t *)ppFrame[i], 1536, (uint8_t*)pTmpImage, 4 * dec.GetWidth(), dec.GetWidth(), dec.GetHeight());
//                OverlayImage((uint8_t *)pTmpImage, dec.GetWidth(), dec.GetHeight(), d_overlay_img_rgba, overlay_width, overlay_height);
                Bgra32ToNv12((uint8_t *)pTmpImage, dec.GetWidth() * 4, (uint8_t*)ppFrame[i], 1536, dec.GetWidth(), dec.GetHeight());
//                GetImage(pTmpImage, reinterpret_cast<uint8_t*>(pImage.get()), 4 * dec.GetWidth(), dec.GetHeight());

                std::vector<std::vector<uint8_t>> vPacket;
                const NvEncInputFrame* encoderInputFrame = pEnc->GetNextInputFrame();

                if ((bOut10 && dec.GetBitDepth() > 8) || (!bOut10 && dec.GetBitDepth() == 8))
                {
                    printf("device frame pitch: %d\n", dec.GetDeviceFramePitch());
                    printf("GetEncodeWidth: %d\n", pEnc->GetEncodeWidth());
                    printf("EcoderInputFrame->pitch: %d\n", encoderInputFrame->pitch);
//                    printf("EcoderInputFrame->bufferFormat: %d\n", encoderInputFrame->bufferFormat);
//                    printf("EcoderInputFrame->bufferFormat: %d\n", encoderInputFrame->chromaOffsets);
//                    printf("EcoderInputFrame->bufferFormat: %d\n", encoderInputFrame->numChromaPlanes);
                    NvEncoderCuda::CopyToDeviceFrame(cuContext,
                        ppFrame[i],
                        encoderInputFrame->pitch, // dec.GetDeviceFramePitch(), WHAT IT USED TO BE, WRONG
                        (CUdeviceptr)encoderInputFrame->inputPtr,
                        encoderInputFrame->pitch,
                        pEnc->GetEncodeWidth(),
                        pEnc->GetEncodeHeight(),
                        CU_MEMORYTYPE_DEVICE,
                        encoderInputFrame->bufferFormat,
                        encoderInputFrame->chromaOffsets,
                        encoderInputFrame->numChromaPlanes);
                    pEnc->EncodeFrame(vPacket);
                }
                else
                {
                    // Bit depth conversion is needed
                    if (bOut10)
                    {
                        ConvertUInt8ToUInt16((uint8_t *)ppFrame[i], (uint16_t *)encoderInputFrame->inputPtr, dec.GetDeviceFramePitch(), encoderInputFrame->pitch,
                            pEnc->GetEncodeWidth(),
                            pEnc->GetEncodeHeight() + ((pEnc->GetEncodeHeight() + 1) / 2));
                    }
                    else
                    {
                        ConvertUInt16ToUInt8((uint16_t *)ppFrame[i], (uint8_t *)encoderInputFrame->inputPtr, dec.GetDeviceFramePitch(), encoderInputFrame->pitch,
                            pEnc->GetEncodeWidth(),
                            pEnc->GetEncodeHeight() + ((pEnc->GetEncodeHeight() + 1) / 2));
                    }
                    pEnc->EncodeFrame(vPacket);
                }
                nFrame += (int)vPacket.size();
                for (std::vector<uint8_t> &packet : vPacket)
                {
                    std::cout << packet.size() << "\t\r";
                    fpOut.write(reinterpret_cast<char*>(packet.data()), packet.size());
                }
            }
        } while (nVideoBytes);

        if (pTmpImage) {
            cuMemFree(pTmpImage);
        }

//        if (d_overlay_img_rgba) {
//            cudaFree(d_overlay_img_rgba);
//        }

        if (pEnc)
        {
            std::vector<std::vector<uint8_t>> vPacket;
            pEnc->EndEncode(vPacket);
            nFrame += (int)vPacket.size();
            for (std::vector<uint8_t> &packet : vPacket)
            {
                std::cout << packet.size() << "\t\r";
                fpOut.write(reinterpret_cast<char*>(packet.data()), packet.size());
            }
            std::cout << std::endl;
        }

        fpIn.close();
        fpOut.close();

        std::cout << "Total frame transcoded: " << nFrame << std::endl << "Saved in file " << szOutFilePath << " of " << (bOut10 ? 10 : 8) << " bit depth" << std::endl;

        return 0;
    }
    catch (const std::exception& ex)
    {
        std::cout << ex.what();
        exit(1);
    }
    return 0;
}
