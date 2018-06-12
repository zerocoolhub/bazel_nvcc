package(default_visibility = ["//visibility:public"])

cc_library(
  name = "cuda_nvcodec",
  srcs = glob(["*.cc", "*.h", "*.cpp"]),
  deps = [
      "@cuda//:main",
      "//cuda_utils:cuda_utils"
  ],
  copts= ["-x cuda"], # Here for now b/c this is how TF crosstool wrapper works
)