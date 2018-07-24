package(default_visibility = ["//visibility:public"])

cc_library(
  name = "cuda_kernels",
  srcs = ["overlay.h", "overlay_kernel.cu.cc", "overlay_kernel.h"],
  deps = ["@cuda//:main"],
  copts= ["-x cuda"], # Here for now b/c this is how TF crosstool wrapper works
)
