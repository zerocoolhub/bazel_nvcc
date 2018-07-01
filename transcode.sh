#!/bin/bash

bazel build //cuda_trans:cuda_trans --spawn_strategy=standalone --crosstool_top=//crosstool:toolchain
./bazel-bin/cuda_trans/cuda_trans -i getfit_input.mp4 -o out.h264
ffmpeg -r 29.97 -i out.h264 -vcodec copy test.mkv
