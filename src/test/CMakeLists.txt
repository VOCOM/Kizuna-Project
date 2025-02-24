project(test)

# Compile Library
add_library(${PROJECT_NAME} src/matrix_test.cpp)
target_include_directories(${PROJECT_NAME}
    PUBLIC inc
    PUBLIC ${CMAKE_SOURCE_DIR}/inc
    PUBLIC ${CMAKE_SOURCE_DIR}/src/core/inc
    PUBLIC ${CMAKE_SOURCE_DIR}/src/utility/inc
)

# Link to App
target_link_libraries(${CMAKE_PROJECT_NAME} ${PROJECT_NAME})
