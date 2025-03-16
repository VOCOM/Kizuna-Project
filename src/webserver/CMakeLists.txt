project(webserver VERSION 1.0)

if(${CMAKE_GENERATOR} MATCHES "Visual Studio")
  file(GLOB SRC_FILES *.cpp)
  if(${CMAKE_VS_PLATFORM_NAME} MATCHES "x64")
    add_compile_definitions(WIN64)
  else()
    add_compile_definitions(WIN32)  
  endif()
endif()

# Compile Library
file(GLOB RSP_FILES responses/*.cpp)
add_library(${PROJECT_NAME} ${SRC_FILES} ${RSP_FILES})
target_include_directories(${PROJECT_NAME} 
  PUBLIC ${CMAKE_SOURCE_DIR}/inc/utility
  PUBLIC ${CMAKE_SOURCE_DIR}/inc/kizuna
  PUBLIC ${CMAKE_SOURCE_DIR}/inc/kizuna/errors
  PUBLIC ${CMAKE_SOURCE_DIR}/inc/webserver
)

# Link to App
target_link_libraries(${CMAKE_PROJECT_NAME} ${PROJECT_NAME})

# HTML Files
file(GLOB RES_FILES index.html)
file(GLOB CSS_FILES *.css)
file(GLOB SVG_FILES svg/*.svg)
file(COPY ${RES_FILES} DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
file(COPY ${CSS_FILES} DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/styles)
file(COPY ${SVG_FILES} DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/svg)

