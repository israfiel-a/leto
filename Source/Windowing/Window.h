/**
 * @file Window.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides Leto's windowing interface, which is an abstraction over
 * the GLFW library. All explanations, errors, and warnings can be found
 * documented with their parent object.
 * @date 2024-08-22
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef __LETO__WINDOW__
#define __LETO__WINDOW__

#include <Diagnostic/Platform.h>
#include <glad.h>
#include <glfw3.h>
#include <stdint.h>

typedef struct
{
    char title[128];
    GLFWwindow* _w;
} window_t;

const window_t* CreateWindow(const char* title);

#endif // __LETO__WINDOW__
