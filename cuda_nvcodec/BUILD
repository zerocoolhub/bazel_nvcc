package(default_visibility = ["//visibility:public"])

config_setting(
    name = "useCUDA",
    values = {
      "copt": "-DuseCUDA",
    },
)

cc_library(
  name = "cuda_nvcodec",
  srcs = select({":useCUDA": glob(["NvDecoder/*", "NvEncoder/*"]),
                 "//conditions:default": []
               }),
  deps = ["@cuda//:main", "//cuda_utils:cuda_utils"],
)

cc_binary(
  name = "main",
  srcs = ["main.cc"],
  deps = [":cuda"],
)
