project(webserver VERSION 1.0)

file(GLOB SRC_FILES *.cpp)
  
if(${WIN32})
  add_compile_definitions(WIN32)  
endif()

# Compile Library
file(GLOB RSP_FILES responses/*.cpp)
add_library(${PROJECT_NAME} ${SRC_FILES} ${RSP_FILES})
target_include_directories(${PROJECT_NAME}
  PUBLIC ${CMAKE_SOURCE_DIR}/inc
  PUBLIC ${CMAKE_SOURCE_DIR}/inc/webserver
  PUBLIC ${CMAKE_SOURCE_DIR}/inc/kizuna
  PUBLIC ${CMAKE_SOURCE_DIR}/inc/kizuna/errors
)

# Link to App
target_link_libraries(${CMAKE_PROJECT_NAME} ${PROJECT_NAME})

# HTML Files
file(GLOB HTML_FILES html/*.html)
file(GLOB JS_FILES scripts/*.js)
file(GLOB CSS_FILES styles/*.css)
file(GLOB SVG_FILES resources/*.svg)
file(COPY ${HTML_FILES} DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_CONFIGURATION_TYPES}/html)
file(COPY ${JS_FILES} DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_CONFIGURATION_TYPES}/scripts)
file(COPY ${CSS_FILES} DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_CONFIGURATION_TYPES}/styles)
file(COPY ${SVG_FILES} DESTINATION ${CMAKE_BINARY_DIR}/${CMAKE_CONFIGURATION_TYPES}/svg)
