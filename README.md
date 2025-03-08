# Kizuna Project
Hub of Specialized AI for home IoT

| Layer    | Language        |
| :------- | :-------------- |
| Frontend | Javascript HTMX |
| Backend  | C++             | 

# Feature Table

| Modules   | Category              | Features       |
| :-------- | :-------------------- | :------------- |
| Kizuna    | Task Queue            | Console Logger |
| Harmony   | Hardware Acceleration | OpenCL         |
|           | Unsupervised          | kMeans         |
| Webserver | Webpage Provider      |                |
|           | Request Forwarding    |                |
| Utility   |                       |                |
| Test      |                       |                |

# Build
```
cmake -B ./build -S .
```

# Build Options
| Parameter     |       |        |
| :------------ | :---: | :----: |
| PLATFORM_TYPE |  CL   |  CUDA  |
| BUILD_TESTS   |  ON   |  OFF   |
