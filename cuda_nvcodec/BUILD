package(default_visibility = ["//visibility:public"])

config_setting(
    name = "useCUDA",
    values = {
      "copt": "-DuseCUDA",
    },
)

cc_library(
  name = "cuda_nvcodec",
  srcs = select({":useCUDA": glob(["*.cc", "*.h", "*.cpp"]),
                 "//conditions:default": []
               }),
  deps = ["@cuda//:main", "//cuda_utils:cuda_utils"],
#    linkopts = ["-lm"],
)

cc_binary(
  name = "main",
  srcs = ["main.cc"],
  deps = [":cuda"],
)
