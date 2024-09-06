/**
 * @file Window.c
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides the implementation of the public interface defined in
 * @file Window.h, along with private values having to do with window
 * information.
 * @date 2024-08-25
 *
 * @copyright (c) 2024 - the Leto Team
 * This document is under the GNU Affero General Public License v3.0. It
 * can be modified and distributed (commercially or otherwise) freely, and
 * can be used privately and within patents. No liability or warranty is
 * guaranteed. However, on use, the user must state license and copyright,
 * any changes made, and disclose the source of the document. For more
 * information see the @file LICENSE.md file included with this
 * distribution of the Leto source code.
 */

#include "Window.h"              // Public interface parent
#include <Diagnostic/Platform.h> // Platform and version macros
#include <Output/Reporter.h>     // Warning and error reporting
#include <Utilities/Strings.h>   // String utilities
#include <gl.h>                  // GLAD2 OpenGL declarations
#include <glfw3.h>               // GLFW3 public interface

/**
 * @brief This is the application window's main structure. It is stored
 * exclusively in this file, and all references that leak from this file
 * are constant. The will be initialized by @ref CreateWindowContext and
 * returned back to NULL via @ref DestroyWindow.
 */
static struct
{
    /**
     * @brief The title of the window. This value is @ref malloc ed by @ref
     * CreateWindow, and @ref free d by @ref DestroyWindow. @note This
     * value is included purely for record purposes. Any changes to this
     * value wil @b not be pushed to the actual window.
     */
    char* title;
    /**
     * @brief The internal GLFW object containing our window's state;
     * width, height, title, contexts, etc. @warning This value should not
     * be edited outside of helper functions, as doing so may cause memory
     * leaks or crashes.
     */
    GLFWwindow* _w;
    /**
     * @brief This value represents the default monitor's video
     * information. This includes values like the height and width of the
     * monitor, the refresh rate, and more.
     */
    const GLFWvidmode* _m;
} application_window = {NULL, NULL, NULL};

/**
 * DESCRIPTION
 *
 * @brief Create the GLFW window object. This is abstracted away from the
 * main window creation method because the underlying functionality of this
 * changes from platform to platform.
 *
 * PARAMETERS
 *
 * @param primary_monitor The primary monitor as reported by GLFW. We
 * assign our window to this monitor if on Linux, if on windows we simply
 * ignore this, opting to manually fullscreen our window instead.
 *
 * RETURN VALUE
 *
 * Nothing of note.
 *
 * WARNINGS
 *
 * Nothing of note.
 *
 * ERRORS
 *
 * @exception glfw_window_create_failed -- If the window is not created
 * successfully by GLFW, this error is thrown and the application quits.
 *
 */
static void CreateWindowObject_(GLFWmonitor* primary_monitor)
{
#if defined(__LETO__LINUX__)
    application_window._w = glfwCreateWindow(
        application_window._m->width, application_window._m->height,
        application_window.title, primary_monitor, NULL);
    if (application_window._w == NULL)
        LetoReportError(glfw_window_create_failed);
#elif defined(__LETO__WINDOWS__)
    (void)primary_monitor;
    application_window._w = glfwCreateWindow(
        application_window._m->width, application_window._m->height,
        application_window.title, NULL, NULL);
    if (application_window._w == NULL)
        LetoReportError(glfw_window_create_failed);
    glfwSetWindowPos(application_window._w, 0, 0);
#endif
}

void LetoCreateWindow(const char* title)
{
    if (application_window._w != NULL)
    {
        LetoReportWarning(double_window_creation);
        return;
    }

    if (!glfwInit()) LetoReportError(glfw_init_failed);
    // OpenGL Core Profile v4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    application_window.title = LetoStringMalloc(127); // 127c + NUL
    LetoSetStringF(true, &application_window.title, 128,
                   "%s | v"__LETO__VERSION__STRING__, title);

    GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
    if (primary_monitor == NULL) LetoReportError(glfw_monitor_get_failed);
    application_window._m = glfwGetVideoMode(primary_monitor);
    if (application_window._m == NULL)
        LetoReportError(glfw_monitor_get_failed);

// Depending on what display server we're using, either set the
// Wayland app ID or the X11 class name.
#if defined(__LETO__WAYLAND__)
    glfwWindowHintString(GLFW_WAYLAND_APP_ID, application_window.title);
#elif defined(__LETO__X11__)
    glfwWindowHintString(GLFW_X11_CLASS_NAME, application_window.title);
    glfwWindowHintString(GLFW_X11_INSTANCE_NAME, application_window.title);
#endif

    CreateWindowObject_(primary_monitor);

    // Make our window's OpenGL context current on this thread.
    glfwMakeContextCurrent(application_window._w);
    if (!gladLoadGL(glfwGetProcAddress))
        LetoReportError(opengl_init_failed);
}

void LetoDestroyWindow(void)
{
    if (application_window._w == NULL)
    {
        LetoReportWarning(preemptive_window_free);
        return;
    }

    LetoStringFree(&application_window.title);
    glfwDestroyWindow(application_window._w);
    application_window._m = NULL;
    application_window._w = NULL;

    glfwTerminate();
}

void LetoSwapBuffers(void)
{
    if (application_window._w == NULL)
    {
        LetoReportWarning(preemptive_buffer_swap);
        return;
    }
    glfwSwapBuffers(application_window._w);
}

bool LetoGetRunState(void)
{
    if (application_window._w == NULL)
    {
        LetoReportWarning(preemptive_window_info);
        return false;
    }
    return !glfwWindowShouldClose(application_window._w);
}

const char* LetoGetTitle(void)
{
    if (application_window.title == NULL)
        LetoReportWarning(preemptive_window_info);
    return application_window.title;
}

uint32_t LetoGetWidth(void)
{
    if (application_window._m == NULL)
    {
        LetoReportWarning(preemptive_window_info);
        return 0;
    }
    return (uint32_t)application_window._m->width;
}

uint32_t LetoGetHeight(void)
{
    if (application_window._m == NULL)
    {
        LetoReportWarning(preemptive_window_info);
        return 0;
    }
    return (uint32_t)application_window._m->height;
}
