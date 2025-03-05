project(webserver VERSION 1.0)

if(${CMAKE_GENERATOR} MATCHES "Visual Studio")
  file(GLOB SRC_FILES win_server.cpp)
  if(${CMAKE_VS_PLATFORM_NAME} MATCHES "x64")
    add_compile_definitions(WIN64)
  else()
    add_compile_definitions(WIN32)  
  endif()
endif()

# Compile Library
add_library(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_SOURCE_DIR}/inc
    PUBLIC ${CMAKE_SOURCE_DIR}/inc/webserver
)

# Link to App
target_link_libraries(${CMAKE_PROJECT_NAME} ${PROJECT_NAME})

# HTML Files
file(COPY index.html DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
