cc_binary(
  name = "qt",
  srcs = ["window.cpp", "main.cpp", "window.h", "slidersgroup.cpp", "slidersgroup.h"],
  deps = [
    "@qt//:qt",
    #"@vulkan//:vulkan", # VULKAN MUST BE LOADED AS A SYSTEM LIBRARY, VIA LD_LIBRARY_PATH,
    # This has to be set in the environment first, can't seem to pass this into bazel
    # Otherwise you'll get an error like "error while loading shared libraries: libvulkan.so.1: cannot open shared object file"
    # See the vulkan tutorial.
    # WE DON'T WANT A DEPENDENCY ON VULKAN HERE, OTHERWISE YOU'LL GET A SIMPLE BUT UNSOLVEABLE ERROR: Segmentation fault
  ],
  linkopts = ['-lz'], # Need zlib
  copts = [],
)