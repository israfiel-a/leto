# Project settings. If the user doesn't have at least CMake 3.29.5,
# fail the application's build.
cmake_minimum_required(VERSION 3.30.2 FATAL_ERROR)
project("Leto" DESCRIPTION "Leto is an open-world driving game." LANGUAGES C)
message(STATUS "Attempting to build a ${CMAKE_BUILD_TYPE} version of ${PROJECT_NAME}.")

# Default flags (release OR debug) for all the operating systems
# the project supports. "Linux" flags include MacOS as well.
set(DEFAULT_FLAGS_LINUX -Wall -Werror -Wpedantic -Wfatal-errors)
set(DEFAULT_FLAGS_WINDOWS /Wall /WX /wd5045 /wd4820 /wd4996 /wd4710 /wd4711)
set(LIBRARY_LIST glad2 glfw3 cglm)

set(LIBRARY_FLAGS_LINUX -Wno-pedantic)
set(LIBRARY_FLAGS_WINDOWS /wd4191 /wd4242)

# Append some flags and do some other calculations should we
# be on a supported operating system. If we're not, this compilation
# process should stop immediately.
if(LINUX OR APPLE)
    set(C_FLAGS_DEBUG ${DEFAULT_FLAGS_LINUX} -g -fsanitize=undefined)
    set(C_FLAGS_RELEASE ${DEFAULT_FLAGS_LINUX} -Ofast)
    set(C_LINKER_FLAGS -fsanitize=undefined)
    list(APPEND LIBRARY_LIST m)

    # If we're running on Linux, we've gotta figure out what
    # windowing server we're running. I've configured CMake to prefer
    # Wayland, as it's very much better than X11 for our purposes.
    if(NOT APPLE)
        message(STATUS "Building for Linux.")
        add_compile_definitions(BUILD_PLATFORM=1)
        find_library(WAYLAND NAMES wayland wayland-client)
        find_library(X11 NAMES x11 X11)

        if(NOT "${WAYLAND}" STREQUAL "WAYLAND-NOTFOUND")
            add_compile_definitions(DISPLAY_SERVER=0)
            set(GLFW_BUILD_X11 false)
            set(GLFW_BUILD_WAYLAND true)
            message(STATUS "Using the Wayland display server.")
        elseif(NOT "${X11}" STREQUAL "X11-NOTFOUND")
            add_compile_definitions(DISPLAY_SERVER=1)
            set(GLFW_BUILD_WAYLAND false)
            set(GLFW_BUILD_X11 true)
            message(STATUS "Using the X11 display server.")
        else()
            message(FATAL_ERROR "Your Linux distribution uses an unsupported"
                " display server. Leto will not be able to run.")
        endif()
    else()
        message(STATUS "Building for MacOS.")
        add_compile_definitions(BUILD_PLATFORM=2)
    endif()
elseif(WIN32)
    add_compile_definitions(BUILD_PLATFORM=0)
    message(STATUS "Building for Windows.")
    set(C_FLAGS_DEBUG ${DEFAULT_FLAGS_WINDOWS} /fsanitize=address)
    set(C_FLAGS_RELEASE ${DEFAULT_FLAGS_WINDOWS} /O2)
else()
    message(FATAL_ERROR "Unsupported operating system. Please find a machine "
    "running either Linux, Windows, or MacOS and recompile the program.")
endif()

# Build directory configuration. This forces CMakes to build
# into a folder within the current build folder named after the
# project.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PROJECT_NAME}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
# Project directory macros, so we can access them easier.
set(DEPENDENCY_DIRECTORY "${CMAKE_SOURCE_DIR}/deps")
set(SOURCE_DIRECTORY "${CMAKE_SOURCE_DIR}/src")
set(RESOURCE_DIRECTORY "${CMAKE_SOURCE_DIR}/rss")

# Print out each of these directories for the user's amusement.
message(STATUS "Build directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}\n"
    "\tDependency directory: ${DEPENDENCY_DIRECTORY}\n\tSource directory: " 
    "${SOURCE_DIRECTORY}\n\tResource directory: ${RESOURCE_DIRECTORY}")

# Set the C standard to compile for. We want C11, which contains
# some features like _Generic that make programming in this archaic
# language a whole lot easier.
set(C_STANDARD 11)
set(C_STANDARD_REQUIRED on)
message(STATUS "Using the ${CMAKE_C_COMPILER_ID} compiler.\n  "
    "\tStandard: C${C_STANDARD}")

# Depending on the target type of binary we're building for, add some
# commands and flags.
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_EXPORT_COMPILE_COMMANDS YES)
    add_compile_definitions(BUILD_TYPE=0)
    add_compile_options(${C_FLAGS_DEBUG})
    add_link_options(${C_LINKER_FLAGS})
else()
    add_compile_definitions(BUILD_TYPE=1)
    add_compile_options(${C_FLAGS_RELEASE})
endif()

# Set some path macros to represent the different libraries we're
# pulling into the project build.
set(GLFW_PATH "${DEPENDENCY_DIRECTORY}/glfw")
set(GLAD_PATH "${DEPENDENCY_DIRECTORY}/glad2")
set(CGLM_PATH "${DEPENDENCY_DIRECTORY}/cglm")

# Include the respective "inc" directories of each one of our dependencies,
# and the source directory of the project. Also specify the link directory,
# which will tell our program where to look first to link libraries.
include_directories("${GLFW_PATH}/inc" "${GLAD_PATH}" "${CGLM_PATH}" 
    "${SOURCE_DIRECTORY}")
link_directories("${CMAKE_BINARY_DIR}")
# Print out these set directories.
message(STATUS "Library Info:\n\tLink Directory: ${CMAKE_BINARY_DIR}\n"
    "\tLibraries: ${LIBRARY_LIST}")
message(STATUS "Dependency Info:\n\tGLFW: ${GLFW_PATH}\n\tGLAD: "
    "${GLAD_PATH}\n\tCGLM: ${CGLM_PATH}")

# Add our dependencies to the project, pulling in each's CMakeList as we need.
# The only dependencies listed here should be those built separately from the main
# program, with their own CMakeLists.txt file.
add_subdirectory("${GLFW_PATH}")
add_subdirectory("${GLAD_PATH}")
add_subdirectory("${CGLM_PATH}")

message(STATUS "Leto source files:")
file(GLOB PROJECT_SOURCES ${SOURCE_DIRECTORY}/*.c
    ${SOURCE_DIRECTORY}/interface/*.c ${SOURCE_DIRECTORY}/diagnostic/*.c
    ${SOURCE_DIRECTORY}/io/*.c ${SOURCE_DIRECTORY}/utilities/*.c
    ${SOURCE_DIRECTORY}/resources/*.c
)
message(STATUS "Leto asset directory: ${ASSET_PATH}")
file(COPY ${RESOURCE_DIRECTORY} DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
add_compile_definitions(ASSET_DIR="./rss")
message(STATUS ${ASSET_DIR})

foreach(file ${PROJECT_SOURCES})
    cmake_path(GET file FILENAME CURRENT_FILENAME)
    set_source_files_properties(${file} PROPERTIES COMPILE_DEFINITIONS 
        FILENAME="${CURRENT_FILENAME}")
    message(NOTICE "  ${CURRENT_FILENAME}")
endforeach()

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
add_dependencies(${PROJECT_NAME} glad2 glfw cglm)
target_link_libraries(${PROJECT_NAME} ${LIBRARY_LIST})
