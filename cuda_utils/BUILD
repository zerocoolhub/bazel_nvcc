package(default_visibility = ["//visibility:public"])

cc_library(
  name = "cuda_utils",
  srcs = ["BitDepth.cu.cc", "ColorSpace.cu.cc", "Logger.h", "NvCodecUtils.h", "FFmpegDemuxer.h",
          "NvEncoderCLIOptions.h"],
  deps = ["@cuda//:main"],
  copts= ["-x cuda"], # Here for now b/c this is how TF crosstool wrapper works
)

cc_binary(
  name = "main",
  srcs = ["main.cc"],
  deps = [":cuda"],
)
