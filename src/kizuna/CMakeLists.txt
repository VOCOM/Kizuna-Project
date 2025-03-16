project(kizuna VERSION 1.0)

# Compile Library
file(GLOB SRC_FILES *.cpp)
add_library(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME}
  PUBLIC ${CMAKE_SOURCE_DIR}/inc
  PUBLIC ${CMAKE_SOURCE_DIR}/inc/kizuna
  PUBLIC ${CMAKE_SOURCE_DIR}/inc/kizuna/errors
)

# Link to App
target_link_libraries(${CMAKE_PROJECT_NAME} ${PROJECT_NAME})
