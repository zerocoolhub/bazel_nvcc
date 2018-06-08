package(default_visibility = ["//visibility:public"])

config_setting(
    name = "useCUDA",
    values = {
      "copt": "-DuseCUDA",
    },
)

cc_library(
  name = "cuda_utils",
  srcs = select({":useCUDA": ["BitDepth.cu.cc", "ColorSpace.cu.cc", "Logger.h", "NvCodecUtils.h", "FFmpegDemuxer.h", "NvEncoderCLIOptions.h"],
                 "//conditions:default": []
               }),
  deps = ["@cuda//:main"],
)

cc_binary(
  name = "main",
  srcs = ["main.cc"],
  deps = [":cuda"],
)
