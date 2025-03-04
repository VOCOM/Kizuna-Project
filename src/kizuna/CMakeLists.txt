project(kizuna VERSION 1.0)

# Link module headers to App
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC inc)

# Compile Library
add_library(${PROJECT_NAME} src/kizuna.cpp src/data.cpp)
target_include_directories(${PROJECT_NAME}
    PUBLIC inc
    PUBLIC ${CMAKE_SOURCE_DIR}/inc
)

# Link to App
target_link_libraries(${CMAKE_PROJECT_NAME} ${PROJECT_NAME})
