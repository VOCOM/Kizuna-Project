project(harmony VERSION 1.0)

# Compile Library
add_library(${PROJECT_NAME} harmony.cpp kmeans.cpp)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_SOURCE_DIR}/inc
    PUBLIC ${CMAKE_SOURCE_DIR}/inc/harmony
)

# Link to App
target_link_libraries(${CMAKE_PROJECT_NAME} ${PROJECT_NAME})

# Kernel Program files
if(PLATFORM_TYPE STREQUAL "CL")
    file(COPY cl/math.cl DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_NAME})
endif()