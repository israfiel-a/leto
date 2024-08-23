message(STATUS "Attempted CMake configuration for Leto on \
with ${CMAKE_BUILD_TYPE} parameters.")

cmake_minimum_required(VERSION 3.30.2)
project("Leto" DESCRIPTION "Leto is a story-driven open world game \
about escaping from a failing authoritarian regime." LANGUAGES C)

find_package(Git REQUIRED)
include(ExternalProject)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_compile_definitions(BUILD_PLATFORM=0)
    add_compile_options(/Wall /WX)

    if(CMAKE_BUILD_TYPE STREQUAL "Debug") 
        set(CMAKE_EXPORT_COMPILE_COMMANDS YES)
        add_compile_options(/Z7 /fsanitize=address)
        add_compile_definitions(BUILD_TYPE=0)
    else()
        add_compile_options(/O2)
        add_compile_definitions(BUILD_TYPE=1)
    endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_compile_definitions(BUILD_PLATFORM=1)
    add_compile_options(-Wall -Werror -Wpedantic -Wfatal-errors)

    find_library(WAYLAND NAMES wayland wayland-client 
        libwayland libwayland-client)
    find_library(X11 NAMES x11 X11 libx11 libX11)

    if(NOT "${WAYLAND}" STREQUAL "WAYLAND-NOTFOUND")
        set(GLFW_LINUX_PLATFORM -DGLFW_BUILD_X11=false)
    elseif(NOT "${X11}" STREQUAL "X11-NOTFOUND")
        set(GLFW_LINUX_PLATFORM -DGLFW_BUILD_WAYLAND=false)
    else()
        message(FATAL_ERROR "Your Linux distribution uses an \
unsupported display server. Leto will not be able to run.")
    endif()

    if(CMAKE_BUILD_TYPE STREQUAL "Debug") 
        set(CMAKE_EXPORT_COMPILE_COMMANDS YES)
        add_compile_options(-g -fsanitize=undefined)
        add_link_options(-fsanitize=undefined)
        add_compile_definitions(BUILD_TYPE=0)
    else()
        add_compile_options(-Ofast)
        add_compile_definitions(BUILD_TYPE=1)
    endif()
else()
    message(FATAL_ERROR "Your system is not compatible with Leto.")
endif()

set(GLFW_PATH "${CMAKE_SOURCE_DIR}/Dependencies/GLFW")
set(GLAD_PATH "${CMAKE_SOURCE_DIR}/Dependencies/GLAD")

ExternalProject_add("GLFW"
    GIT_REPOSITORY "https://github.com/glfw/glfw"
    TMP_DIR "${GLFW_PATH}/tmp" STAMP_DIR "${GLFW_PATH}/tmp"
    DOWNLOAD_DIR "${GLFW_PATH}/src" SOURCE_DIR "${GLFW_PATH}/src"
    BINARY_DIR "${GLFW_PATH}"
    INSTALL_COMMAND ""
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} 
        -DGLFW_BUILD_DOCS=false -DGLFW_BUILD_TESTS=false 
        -DGLFW_BUILD_EXAMPLES=false ${GLFW_LINUX_PLATFORM})

include_directories("${GLFW_PATH}/src/include/GLFW" "${GLAD_PATH}" 
    "${CMAKE_SOURCE_DIR}/Source")
link_directories("${GLFW_PATH}/src/")

set(CMAKE_C_STANDARD 11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY 
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

file(GLOB PROJECT_SOURCES 
    ${CMAKE_SOURCE_DIR}/Source/*.c
    ${CMAKE_SOURCE_DIR}/Source/Windowing/*.c
    ${CMAKE_SOURCE_DIR}/Source/Diagnostic/*.c
    ${CMAKE_SOURCE_DIR}/Source/Output/*.c
    ${CMAKE_SOURCE_DIR}/Source/Utilities/*.c
)

foreach(file ${PROJECT_SOURCES})
    cmake_path(GET file FILENAME CURRENT_FILENAME)
    set_source_files_properties(${file} PROPERTIES COMPILE_DEFINITIONS 
        FILENAME="${CURRENT_FILENAME}")
endforeach()

add_subdirectory("${GLAD_PATH}")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
add_dependencies(${PROJECT_NAME} GLFW)
target_link_libraries(${PROJECT_NAME} glad glfw3 m)