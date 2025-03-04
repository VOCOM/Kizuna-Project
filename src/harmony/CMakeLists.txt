project(harmony VERSION 1.0)

# Link module headers to App
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC inc)

# Compile Library
add_library(${PROJECT_NAME} src/harmony.cpp src/kmeans.cpp)
target_include_directories(${PROJECT_NAME}
    PUBLIC inc
    PUBLIC ${CMAKE_SOURCE_DIR}/inc
    PUBLIC ${CMAKE_SOURCE_DIR}/src/kizuna/inc
    PUBLIC ${CMAKE_SOURCE_DIR}/src/utility/inc
)

# Link to App
target_link_libraries(${CMAKE_PROJECT_NAME} ${PROJECT_NAME})

# Kernel Program files
if(PLATFORM_TYPE STREQUAL "CL")
    file(COPY cl/math.cl DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${PROJECT_NAME})
endif()