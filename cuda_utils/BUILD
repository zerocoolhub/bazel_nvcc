package(default_visibility = ["//visibility:public"])

cc_library(
  name = "cuda_utils",
  srcs = [
      "BitDepth.cu.cc",
      "ColorSpace.cu.cc",
      "FFmpegDemuxer.h",
      "Logger.h",
      "NvCodecUtils.h", 
      "NvEncoderCLIOptions.h",
      "Resize.cu.cc"
  ],
  deps = ["@cuda//:main"],
  copts= ["-x cuda"], # Here for now b/c this is how TF crosstool wrapper works
)
