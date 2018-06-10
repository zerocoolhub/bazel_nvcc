
# Building + Running

```
bazel run //cuda_trans:cuda_trans --spawn_strategy=standalone --crosstool_top=//crosstool:toolchain --copt=-DuseCUDA
```

## Explanation

* **--spawn_strategy=standalone**
  * Required because the _CROSSTOOL_ file references the file _crosstool_wrapper_driver_is_not_gcc_, which Bazel does not make available to it due to Sandboxing. This deactivates the sandbox and makes the file accessible.

* **--crosstool_top=//crosstool:toolchain**
  * From Bazel: This option specifies the location of the crosstool compiler suite to be used for all C++ compilation during a build. Bazel will look in that location for a CROSSTOOL file and uses that to automatically determine settings for --compiler
