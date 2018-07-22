package(default_visibility = ["//visibility:public"])

cc_library(
    name = "qt",
    srcs = glob(
        ["lib/**/*"], # You need to include all .so files, otherwise you'll get some weird linker warning about Q_5_Version or something
        exclude = ["lib/**/*.pc", "lib/**/*.prl", "lib/**/*.la", "lib/**/*.cmake",
        # These excludes must be here otherwise you'll get duplicate symbol linker errors
        # with NVCC. It was an educated guess to try and exclude the offending modules and hope
        # the qt library still works, which it does for the components of QT that we use.
        "lib/**/libQt5MultimediaQuick*", "lib/**/libQt5QuickParticles*", "lib/**/libQt5QuickTemplates2*"],
    ),
    hdrs = glob([
        "include/**/*", # So that we can pick up files like QGuiApplication (w/ no extension)
    ]),
    includes = ["include/"],
    #linkopts = ["-Wl,-rpath,/usr/local/cuda/lib64"],
)