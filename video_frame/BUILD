load("//:cuda.bzl", "cc_cuda_binary")
package(default_visibility = ["//visibility:public"])

cc_cuda_binary(
  name = "video_frame",
  srcs = glob(["*.cpp", "*.h"]),
  deps = [
    "@qt//:qt",
    #"@vulkan//:vulkan", # VULKAN MUST BE LOADED AS A SYSTEM LIBRARY, VIA LD_LIBRARY_PATH,
    # This has to be set in the environment first, can't seem to pass this into bazel
    # Otherwise you'll get an error like "error while loading shared libraries: libvulkan.so.1: cannot open shared object file"
    # See the vulkan tutorial.
    # WE DON'T WANT A DEPENDENCY ON VULKAN HERE, OTHERWISE YOU'LL GET A SIMPLE BUT UNSOLVEABLE ERROR: Segmentation fault
  ],
  linkopts = ['-lz', '-fPIE',
  "-L/usr/local/lib", "-lopencv_dnn", "-lopencv_ml", "-lopencv_shape", "-lopencv_stitching",
  "-lopencv_superres", "-lopencv_videostab",
  "-lopencv_calib3d", "-lopencv_features2d", "-lopencv_highgui", "-lopencv_videoio",
  "-lopencv_photo", "-lopencv_imgcodecs",
  "-lopencv_cudafilters", "-lopencv_video", "-lopencv_objdetect", "-lopencv_imgproc",
  "-lopencv_flann", "-lopencv_viz", "-lopencv_core",
  ], # Need zlib, otherwise will get linker error: undefined reference to 'compress2'
  # copts = ['-fPIC'],
  copts = ["-fPIC", "-x cuda"], # Here for now b/c this is how TF crosstool wrapper works
)