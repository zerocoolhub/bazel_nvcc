package(default_visibility = ["//visibility:public"])

cc_library(
    name = "qt",
    srcs = glob(
        ["lib/**/*"], # You need to include all .so files, otherwise you'll get some weird linker warning about Q_5_Version or something
        exclude = ["lib/**/*.pc", "lib/**/*.prl", "lib/**/*.la", "lib/**/*.cmake"],
    ),
    hdrs = glob([
        "include/**/*", # So that we can pick up files like QGuiApplication (w/ no extension)
    ]),
    includes = ["include/"],
    #linkopts = ["-Wl,-rpath,/usr/local/cuda/lib64"],
)