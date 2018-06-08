package(default_visibility = ["//visibility:public"])

config_setting(
    name = "useCUDA",
    values = {
      "copt": "-DuseCUDA",
    },
)

cc_binary(
  name = "cuda_trans",
  srcs = select({":useCUDA": ["AppTrans.cpp"],
                 "//conditions:default": ["AppTrans.cpp"]
               }),
  deps = ["@cuda//:main", "//cuda_utils:cuda_utils", "//cuda_nvcodec:cuda_nvcodec"],
#      linkopts = ["-lm"],
  linkopts = ["-lavcodec", "-lavutil", "-lavformat"],
)

cc_binary(
  name = "main",
  srcs = ["main.cc"],
  deps = [":cuda"],
)
