# Kizuna Project
Hub of Specialized AI for home IoT

| Modules   | Category              |            |
| :-------- | :-------------------- | :--------- |
| Kizuna    | Console Logger        |            |
|           | Task Queue            |            |
| Harmony   | Hardware Acceleration |            |
|           | Unsupervised          | kMeans     |
| Webserver | Webpage Provider      |            |
|           | Request Forwarding    |            |
| Utility   | Timer                 |            |
| Test      |                       |            |

# Build
```
cmake -B ./build -S .
```

# Build Options
| Parameter     |       |        |
| :------------ | :---: | :----: |
| PLATFORM_TYPE |  CL   |  CUDA  |
| BUILD_TESTS   |  ON   |  OFF   |
