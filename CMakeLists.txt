cmake_minimum_required(VERSION 3.20)
project(Kizuna VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Build Variant
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

# Default Hardware Acceleration Variant
if(NOT DEFINED PLATFORM_TYPE)
    set(PLATFORM_TYPE "CL")
endif()

# Define Target
add_executable(${CMAKE_PROJECT_NAME} src/main.cpp src/kizuna/submodule.cpp)

# Configure Modules
add_subdirectory(src/utility)
add_subdirectory(src/kizuna)
add_subdirectory(src/webserver)
add_subdirectory(src/harmony)

if(BUILD_TEST STREQUAL "ON")
    add_subdirectory(src/test)
endif()

# Link 3rd Party Libraries
if(PLATFORM_TYPE STREQUAL "CUDA")
    message("CUDA not yet implemented.")
else()
    target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/OpenCL.lib)
endif()

# Configuration file
file(COPY config.ini DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})