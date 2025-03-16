project(sqlite VERSION 3.49.1)

# Compile Library
file(GLOB SRC_FILES *.cpp *.c)
add_library(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME}
  PUBLIC ${CMAKE_SOURCE_DIR}/inc/utility
  PUBLIC ${CMAKE_SOURCE_DIR}/inc/kizuna
  PUBLIC ${CMAKE_SOURCE_DIR}/inc/kizuna/errors
  PUBLIC ${CMAKE_SOURCE_DIR}/inc/database
  PUBLIC ${CMAKE_SOURCE_DIR}/inc/database/sqlite
)

# Link to App
target_link_libraries(${CMAKE_PROJECT_NAME} ${PROJECT_NAME})
