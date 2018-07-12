#!/bin/bash

bazel build //image_provider:image_provider --spawn_strategy=standalone --crosstool_top=//crosstool:toolchain
./bazel-bin/image_provider/image_provider -i getfit_input.mp4 -o out.h264 -of bgra
