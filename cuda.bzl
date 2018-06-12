def register_extension_info(**kwargs):
    pass

def cc_cuda_library(deps=None, **kwargs):
  if not deps:
    deps = []

  native.cc_library(
    deps = deps + ["@cuda//:main"],
    **kwargs)

register_extension_info(
    extension_name = "cc_cuda_library",
    label_regex_for_dep = "{extension_name}",
)

def cc_cuda_binary(deps=None, **kwargs):
  if not deps:
    deps = []

  native.cc_binary(
    deps = deps + ["@cuda//:main"],
    **kwargs)

register_extension_info(
    extension_name = "cc_cuda_binary",
    label_regex_for_dep = "{extension_name}",
)