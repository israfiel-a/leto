cmake_minimum_required(VERSION 3.29.5)
project("glad" LANGUAGES C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_compile_options(-Ofast -Wno-pedantic)
else()
    add_compile_options(/wd4191)
endif()
add_library(${PROJECT_NAME} egl.c gl.c)
