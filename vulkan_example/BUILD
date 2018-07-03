cc_binary(
  name = "vulkan",
  srcs = ["main.cpp", "stb_image.h"],
  deps = [
    #"@qt//:qt",
    #"@vulkan//:vulkan", # VULKAN MUST BE LOADED AS A SYSTEM LIBRARY, VIA LD_LIBRARY_PATH,
    # This has to be set in the environment first, can't seem to pass this into bazel
    # Otherwise you'll get an error like "error while loading shared libraries: libvulkan.so.1: cannot open shared object file"
    # See the vulkan tutorial.
    # WE DON'T WANT A DEPENDENCY ON VULKAN HERE, OTHERWISE YOU'LL GET A SIMPLE BUT UNSOLVEABLE ERROR: Segmentation fault
  ],
  linkopts = ["-L/usr/local/lib -lglfw3 -lrt -lm -ldl -lX11 -lpthread -lxcb -lXau -lXdmcp -lvulkan"],
  copts = [],
)