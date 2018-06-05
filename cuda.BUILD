package(default_visibility = ["//visibility:public"])

cc_library(
    name = "main",
    srcs = [
        "lib64/libcublas.so.9.2",
        "lib64/libcudart.so.9.2",
        "lib64/libcufft.so.9.2",
        "lib64/libcurand.so.9.2",
    ],
    hdrs = glob([
        "include/**/*.h",
        "include/**/*.hpp",
        "include/**/*.inl",
    ]),
    includes = ["include/"],
    linkopts = ["-Wl,-rpath,/usr/local/cuda/lib64"],
)
