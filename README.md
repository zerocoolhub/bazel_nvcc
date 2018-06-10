
# Building + Running

```
bazel run //cuda_trans:cuda_trans --spawn_strategy=standalone --crosstool_top=//crosstool:toolchain --copt=-DuseCUDA
```

## Explanation
