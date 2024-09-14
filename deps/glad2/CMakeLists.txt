cmake_minimum_required(VERSION 3.29.5)
project("glad2" LANGUAGES C)

# There are some flags we specifically need to enable, since the code
# written within this library does not necessarily conform to all
# standards. 
if(LINUX) 
    add_compile_options(${LIBRARY_FLAGS_LINUX})
elseif(WIN32) 
    add_compile_options(${LIBRARY_FLAGS_WINDOWS})
endif()

add_library(${PROJECT_NAME} egl.c gl.c)
