cmake_minimum_required(VERSION 3.30.2 FATAL_ERROR)
message(STATUS "Attempting to build a ${CMAKE_BUILD_TYPE} version of Leto.")
project("Leto" DESCRIPTION "Leto is an open-world driving game." LANGUAGES C)
message(STATUS "Project Info:\n  Name: ${PROJECT_NAME}\n  Description: "
    "${PROJECT_DESCRIPTION}\n  Author: Israfiel")

set(C_STANDARD 11)
set(C_STANDARD_REQUIRED on)
message(STATUS "Using the ${CMAKE_C_COMPILER_ID} compiler.\n  "
    "Standard: C${C_STANDARD}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
message(STATUS "Build directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

set(LINUX_COMPILATION_FLAGS -Wall -Werror -Wpedantic -Wfatal-errors)
set(WINDOWS_COMPILATION_FLAGS /Wall /WX)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_EXPORT_COMPILE_COMMANDS YES)
    add_compile_definitions(BUILD_TYPE=0)
    list(APPEND LINUX_COMPILATION_FLAGS -g -fsanitize=undefined)
    list(APPEND WINDOWS_COMPILATION_FLAGS /Z7 /fsanitize=address)
    set(LINUX_LINKING_FLAGS -fsanitize=undefined)

    message(NOTICE "  Compile Commands: ../compile_commands.json")
else()
    add_compile_definitions(BUILD_TYPE=1)
    list(APPEND LINUX_COMPILATION_FLAGS -Ofast)
    list(APPEND WINDOWS_COMPILATION_FLAGS /O2)
endif()

macro(define_display_server)
    if(NOT "${WAYLAND}" STREQUAL "WAYLAND-NOTFOUND")
        add_compile_definitions(__LETO__WAYLAND__)
        set(GLFW_BUILD_X11 false)
        set(GLFW_BUILD_WAYLAND true)
        message(STATUS "Using the Wayland display server.")
    elseif(NOT "${X11}" STREQUAL "X11-NOTFOUND")
        add_compile_definitions(__LETO__X11__)
        set(GLFW_BUILD_WAYLAND false)
        set(GLFW_BUILD_X11 true)
        message(STATUS "Using the X11 display server.")
    else()
        message(FATAL_ERROR "Your Linux distribution uses an "
            "unsupported display server. Leto will not be able to run.")
    endif()
endmacro()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_compile_definitions(BUILD_PLATFORM=0)
    add_compile_options(${WINDOWS_COMPILATION_FLAGS})
    message(STATUS "Compiling for Windows.")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_compile_definitions(BUILD_PLATFORM=1)
    add_compile_options(${LINUX_COMPILATION_FLAGS})
    add_link_options(${LINUX_LINKING_FLAGS})
    message(STATUS "Compiling for Linux.")

    find_library(WAYLAND NAMES wayland wayland-client)
    find_library(X11 NAMES x11 X11)
    define_display_server()
else()
    message(FATAL_ERROR "Your system is not compatible with Leto.")
endif()

set(GLFW_PATH "${CMAKE_SOURCE_DIR}/Dependencies/GLFW")
set(GLAD_PATH "${CMAKE_SOURCE_DIR}/Dependencies/GLAD")
set(SOURCE_PATH "${CMAKE_SOURCE_DIR}/Source")
include_directories("${GLFW_PATH}/include/GLFW" "${GLAD_PATH}" "${SOURCE_PATH}")
set(LIBRARY_LIST glad glfw3 m)
message(STATUS "Dependency Info:\n  GLFW: ${GLFW_PATH}\n  GLAD: "
    "${GLAD_PATH}\n  Leto: ${SOURCE_PATH}\n  Linked Libraries: ${LIBRARY_LIST}")

set(GLFW_BUILD_DOCS false)
set(GLFW_BUILD_TESTS false)
set(GLFW_BUILD_EXAMPLES false)
message(STATUS "Adding GLFW to the project. Options:\n  "
    "Documentation: ${GLFW_BUILD_DOCS}\n  Tests: ${GLFW_BUILD_TESTS}\n  "
    "Examples: ${GLFW_BUILD_EXAMPLES}\n  Wayland: ${GLFW_BUILD_WAYLAND}\n  "
    "X11: ${GLFW_BUILD_X11}")

add_subdirectory("${GLFW_PATH}")
add_subdirectory("${GLAD_PATH}")

message(STATUS "Leto source files:")
file(GLOB PROJECT_SOURCES ${CMAKE_SOURCE_DIR}/Source/*.c
    ${CMAKE_SOURCE_DIR}/Source/Windowing/*.c ${CMAKE_SOURCE_DIR}/Source/Diagnostic/*.c
    ${CMAKE_SOURCE_DIR}/Source/Output/*.c ${CMAKE_SOURCE_DIR}/Source/Utilities/*.c
)
foreach(file ${PROJECT_SOURCES})
    cmake_path(GET file FILENAME CURRENT_FILENAME)
    set_source_files_properties(${file} PROPERTIES COMPILE_DEFINITIONS 
        FILENAME="${CURRENT_FILENAME}")
    message(NOTICE "  ${CURRENT_FILENAME}")
endforeach()

message(NOTICE "")
message(NOTICE "")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBRARY_LIST})
