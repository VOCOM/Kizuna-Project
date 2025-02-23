# Kizuna Project
Hub of Specialized AI for home IoT

| Modules | Category              |        |
| :------ | :-------------------- | :----- |
| Core    | Hardware Acceleration |        |
|         | Matrix Suite          |        |
| Models  | Unsupervised          | kMeans |
| Utility | Timer                 |        |
| Test    | Test Suite            |        |

# Build
```
cmake -B ./build -S .
```

# Build Options
| Parameter     |       |        |
| :------------ | :---: | :----: |
| PLATFORM_TYPE |  CL   |  CUDA  |
| BUILD_TESTS   |  ON   |  OFF   |
