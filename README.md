
# Building + Running

```
bazel run //cuda_trans:cuda_trans --spawn_strategy=standalone --crosstool_top=//crosstool:toolchain
```

## Explanation

* **--spawn_strategy=standalone**
  * Required because the _CROSSTOOL_ file references the file _crosstool_wrapper_driver_is_not_gcc_, which Bazel does not make available to it due to Sandboxing. This deactivates the sandbox and makes the file accessible.

* **--crosstool_top=//crosstool:toolchain**
  * From Bazel: This option specifies the location of the crosstool compiler suite to be used for all C++ compilation during a build. Bazel will look in that location for a CROSSTOOL file and uses that to automatically determine settings for --compiler

# NOTES

Some QT shared object files are causing linker errors (duplicate symbols) when compiling with NVCC.
This is fixed by simply excluding the .so files from the definition of the QT bazel repo.

Also, there is some issue with Position Independent Code regarding QT+NVCC, but passing in -fPIC to bazel
through copts doesn't seem to work, you need to add:

-shared -XCompiler -fPIC

in `tf_crosstool_wrapper_driver_is_not_gcc` in the part where you're constructing the full NVCC command. I had
to look at an NVIDIA forum to figure out how to get nvcc to actually respond to this compiler flag.