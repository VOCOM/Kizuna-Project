project(test)

# Compile Library
# add_library(${PROJECT_NAME} src/matrix_test.cpp)
# target_include_directories(${PROJECT_NAME}
#     PUBLIC inc
#     PUBLIC ${CMAKE_SOURCE_DIR}/inc
#     PUBLIC ${CMAKE_SOURCE_DIR}/src/core/inc
#     PUBLIC ${CMAKE_SOURCE_DIR}/src/utility/inc
# )

# Link to App
# target_link_libraries(${CMAKE_PROJECT_NAME} ${PROJECT_NAME})

# Test Data
if(BUILD_TEST STREQUAL "ON")
  file(GLOB TEST_FILES csv/*.csv)
  file(COPY ${TEST_FILES} DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/test)
endif()