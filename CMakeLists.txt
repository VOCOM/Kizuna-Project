cmake_minimum_required(VERSION 3.20)
project(Kizuna VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

if(NOT DEFINED PLATFORM_TYPE)
    set(PLATFORM_TYPE "CL")
endif()

# Define Target
add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)

# Configure Modules
add_subdirectory(src/core)
add_subdirectory(src/utility)
add_subdirectory(src/models)

if(BUILD_TEST STREQUAL "ON")
    add_subdirectory(src/test)
endif()

# Link 3rd Party Libraries
if(PLATFORM_TYPE STREQUAL "CUDA")
    message("CUDA not yet implemented.")
else()
    target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/OpenCL.lib)
endif()
