load("//:cuda.bzl", "cc_cuda_binary")
package(default_visibility = ["//visibility:public"])

cc_cuda_binary(
  name = "cuda_trans",
  srcs = ["AppTrans.cpp"],
  deps = [
#      "@cuda//:main",
      "//cuda_utils:cuda_utils",
      "//cuda_nvcodec:cuda_nvcodec"
  ],
  linkopts = ["-lavcodec", "-lavutil", "-lavformat",
  "-L/usr/local/lib", "-lopencv_dnn", "-lopencv_ml", "-lopencv_shape", "-lopencv_stitching", "-lopencv_cudafeatures2d",
  "-lopencv_superres", "-lopencv_cudacodec", "-lopencv_videostab", "-lopencv_cudaoptflow",
  "-lopencv_cudalegacy", "-lopencv_calib3d", "-lopencv_features2d", "-lopencv_highgui", "-lopencv_videoio",
  "-lopencv_photo", "-lopencv_imgcodecs", "-lopencv_cudawarping", "-lopencv_cudaimgproc",
  "-lopencv_cudafilters", "-lopencv_video", "-lopencv_objdetect", "-lopencv_imgproc",
  "-lopencv_flann", "-lopencv_cudaarithm", "-lopencv_viz", "-lopencv_core", "-lopencv_cudev",
  "-ldl",
  "-lcuda",
  "-lnvcuvid"],
  copts = ["-x cuda"], # Here for now b/c this is how TF crosstool wrapper works
)
