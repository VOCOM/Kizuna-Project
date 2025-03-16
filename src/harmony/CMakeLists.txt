project(harmony VERSION 1.0)

# Compile Library
file(GLOB SRC_FILES *.cpp neuralnet/*.cpp)
add_library(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME}
  PUBLIC ${CMAKE_SOURCE_DIR}/inc
  PUBLIC ${CMAKE_SOURCE_DIR}/inc/harmony
  PUBLIC ${CMAKE_SOURCE_DIR}/inc/harmony/neuralnet
)

# Error Emitter
target_link_libraries(${PROJECT_NAME} kizuna)

# Link to App
target_link_libraries(${CMAKE_PROJECT_NAME} ${PROJECT_NAME})

# Kernel Program files
if(PLATFORM_TYPE STREQUAL "CL")
    file(COPY cl/math.cl DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_NAME})
endif()
