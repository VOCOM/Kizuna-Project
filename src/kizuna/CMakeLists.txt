project(kizuna VERSION 1.0)

# Compile Library
add_library(${PROJECT_NAME} kizuna.cpp data.cpp)
target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_SOURCE_DIR}/inc
    PUBLIC ${CMAKE_SOURCE_DIR}/inc/kizuna
)

# Link to App
target_link_libraries(${CMAKE_PROJECT_NAME} ${PROJECT_NAME})
