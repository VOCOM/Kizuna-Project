project(webserver VERSION 1.0)

# Link module headers to App
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC inc)

if(${CMAKE_GENERATOR} MATCHES "Visual Studio")
  file(GLOB SRC_FILES src/win_server.cpp)
  if(${CMAKE_VS_PLATFORM_NAME} MATCHES "x64")
    add_compile_definitions(WIN64)
  else()
    add_compile_definitions(WIN32)  
  endif()
endif()

# Compile Library
add_library(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME}
    PUBLIC inc
    PUBLIC ${CMAKE_SOURCE_DIR}/inc
)

# Link to App
target_link_libraries(${CMAKE_PROJECT_NAME} ${PROJECT_NAME})

# HTML Files
file(COPY index.html DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
