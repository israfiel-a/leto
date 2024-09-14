cmake_minimum_required(VERSION 3.8.2)
project(cglm
  VERSION 0.9.4
  HOMEPAGE_URL https://github.com/recp/cglm
  DESCRIPTION "OpenGL Mathematics (glm) for C"
  LANGUAGES C
)

# There are some flags we specifically need to enable, since the code
# written within this library does not necessarily conform to all
# standards. 
if(LINUX) 
    add_compile_options(${LIBRARY_FLAGS_LINUX})
elseif(WIN32) 
    add_compile_options(${LIBRARY_FLAGS_WINDOWS})
endif()

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED YES)
set(DEFAULT_BUILD_TYPE "Release")

set(CGLM_BUILD)
option(CGLM_SHARED "Shared build" OFF)
option(CGLM_STATIC "Static build" ON)
option(CGLM_USE_C99 "" OFF)
option(CGLM_USE_TEST "Enable Tests" OFF)

if(CMAKE_SYSTEM_NAME STREQUAL WASI)
  set(CGLM_STATIC ON CACHE BOOL "Static option" FORCE)
  set(CGLM_SHARED OFF CACHE BOOL "Shared option" FORCE)
endif()

if(NOT CGLM_STATIC AND CGLM_SHARED)
  set(CGLM_BUILD SHARED)
else(CGLM_STATIC)
  set(CGLM_BUILD STATIC)
endif()

if(CGLM_USE_C99)
  set(CMAKE_C_STANDARD 99)
endif()

if(MSVC)
  add_definitions(-D_WINDOWS -D_USRDLL)

  if(NOT CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DNDEBUG)
    add_compile_options(/W3 /Ox /Gy /Oi /TC)
    foreach(flag_var
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE
        CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO)
      string(REGEX REPLACE "/RTC(su|[1su])" "" ${flag_var} "${${flag_var}}")
    endforeach(flag_var)
  endif()
else()
  add_compile_options(-Wall)

  if(NOT CMAKE_BUILD_TYPE MATCHES Debug)
    add_compile_options(-O3)
  endif()
endif()

get_directory_property(hasParent PARENT_DIRECTORY)

if(NOT hasParent AND NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

include(GNUInstallDirs)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

if(NOT CPack_CMake_INCLUDED)
  include(CPack)
endif()

# Target Start
add_library(${PROJECT_NAME}
  ${CGLM_BUILD}
  euler.c
  affine.c
  io.c
  quat.c
  cam.c
  vec2.c
  ivec2.c
  vec3.c
  ivec3.c
  vec4.c
  ivec4.c
  mat2.c
  mat2x3.c
  mat2x4.c
  mat3.c
  mat3x2.c
  mat3x4.c
  mat4.c
  mat4x2.c
  mat4x3.c
  plane.c
  frustum.c
  box.c
  project.c
  sphere.c
  ease.c
  curve.c
  bezier.c
  ray.c
  affine2d.c
  clipspace/ortho_lh_no.c
  clipspace/ortho_lh_zo.c
  clipspace/ortho_rh_no.c
  clipspace/ortho_rh_zo.c
  clipspace/persp_lh_no.c
  clipspace/persp_lh_zo.c
  clipspace/persp_rh_no.c
  clipspace/persp_rh_zo.c
  clipspace/view_lh_no.c
  clipspace/view_lh_zo.c
  clipspace/view_rh_no.c
  clipspace/view_rh_zo.c
  clipspace/project_no.c
  clipspace/project_zo.c
  )

if(CGLM_SHARED)
  add_definitions(-DCGLM_EXPORTS)
else()
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DCGLM_STATIC)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
                              VERSION ${PROJECT_VERSION} 
                            SOVERSION ${PROJECT_VERSION_MAJOR})

if(WIN32)
  # Because SOVERSION has no effect to file naming on Windows
  set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_NAME ${PROJECT_NAME}-${PROJECT_VERSION_MAJOR})
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC 
        $<INSTALL_INTERFACE:include>    
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Target for header-only usage
add_library(${PROJECT_NAME}_headers INTERFACE)
target_include_directories(${PROJECT_NAME}_headers INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR})

# Test Configuration
if(CGLM_USE_TEST)
  include(CTest)
  enable_testing()
  add_subdirectory(test)
endif()

# Install 
install(TARGETS ${PROJECT_NAME}
        EXPORT  ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        PATTERN ".*" EXCLUDE)

# Config
export(TARGETS ${PROJECT_NAME}
       NAMESPACE ${PROJECT_NAME}::
       FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
)

install(EXPORT      ${PROJECT_NAME}
        FILE        "${PROJECT_NAME}Config.cmake"
        NAMESPACE   ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

set(PACKAGE_NAME ${PROJECT_NAME})
set(prefix ${CMAKE_INSTALL_PREFIX})
set(exec_prefix ${CMAKE_INSTALL_PREFIX})
if (IS_ABSOLUTE "${CMAKE_INSTALL_INCLUDEDIR}")
  set(includedir "${CMAKE_INSTALL_INCLUDEDIR}")
else()
  set(includedir "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}")
endif()
if (IS_ABSOLUTE "${CMAKE_INSTALL_LIBDIR}")
  set(libdir "${CMAKE_INSTALL_LIBDIR}")
else()
  set(libdir "\${exec_prefix}/${CMAKE_INSTALL_LIBDIR}")
endif()
set(PACKAGE_VERSION "${PROJECT_VERSION}")
configure_file(cglm.pc.in cglm.pc @ONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cglm.pc
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
