
[![Build Status](https://travis-ci.org/Hibbert-pku/bazel_nvcc.svg?branch=master)](https://travis-ci.org/Hibbert-pku/bazel_nvcc)

# Building
bazel run cuda:main --spawn_strategy=standalone --crosstool_top=//crosstool:toolchain --copt=-DuseCUDA

# bazel_nvcc
use nvcc compiler in bazel

# how to build

bazel build cuda:all

bazel test tests:all
