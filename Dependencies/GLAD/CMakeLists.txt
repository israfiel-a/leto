cmake_minimum_required(VERSION 3.30.2)
project("glad" LANGUAGES C)

add_compile_options(-Ofast -Wno-pedantic)
add_library(${PROJECT_NAME} "glad.c")
