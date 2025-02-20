project(core VERSION 1.0)

add_executable(${CMAKE_PROJECT_NAME} main.cpp)

include_directories(${CMAKE_PROJECT_NAME} ${CMAKE_SOURCE_DIR}/inc)

target_link_libraries(${CMAKE_PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/OpenCL.lib)