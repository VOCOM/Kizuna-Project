cmake_minimum_required(VERSION 3.20)
project(Kizuna VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Build Variant
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_compile_definitions(DEBUG)
endif()

if(PLATFORM_TYPE STREQUAL "CUDA")
  add_compile_definitions(CUDA)
  find_package(CUDAToolkit REQUIRED)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/external)

# SDL3
FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG        release-3.2.20
)
FetchContent_MakeAvailable(SDL3)

# Define Target
file(GLOB SRC_FILES src/kizuna/*.cpp)
add_executable(${CMAKE_PROJECT_NAME} ${SRC_FILES})
target_include_directories(${CMAKE_PROJECT_NAME}
  PUBLIC ${CMAKE_SOURCE_DIR}/inc
  PUBLIC ${CMAKE_SOURCE_DIR}/inc/kizuna
  PUBLIC ${CMAKE_SOURCE_DIR}/inc/kizuna/errors
)

# Configure Modules
add_subdirectory(src/utility)
# add_subdirectory(src/webserver)
# add_subdirectory(src/sqlite)
# add_subdirectory(src/harmony)

# Link 3rd Party Libraries
target_link_libraries(${CMAKE_PROJECT_NAME} 
  PRIVATE SDL3::SDL3
  PRIVATE opengl32 # WIN32 only
  # PRIVATE CUDA::cudart
  # PRIVATE ${CMAKE_SOURCE_DIR}/lib/OpenCL.lib
)

# Configuration file
if(CMAKE_GENERATOR MATCHES "Visual Studio")
else()
  set(CONFIG_OUTPUT "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_CONFIGURATION_TYPES}")
  file(COPY config.ini DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${CMAKE_CONFIGURATION_TYPES})
endif()

if(BUILD_TEST STREQUAL "ON")
  add_subdirectory(src/test)
endif()