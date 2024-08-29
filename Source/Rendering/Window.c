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
 * @brief The structure that contains all information to be associated with
 * our application's window, like the @struct GLFWwindow parent object, the
 * window's title, and the user's primary monitor's information.
 */
typedef struct
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
} window_t;

/**
 * @brief This is the application window's main structure. It is stored
 * exclusively in this file, and all references that leak from this file
 * are constant. The will be initialized by @ref CreateWindowContext and
 * returned back to NULL via @ref DestroyWindow.
 */
static window_t application_window = {NULL, NULL, NULL};

void CreateWindow(const char* title)
{
    if (application_window._w != NULL)
    {
        ReportWarning(double_window_creation);
        return;
    }

    if (!glfwInit()) ReportError(glfw_init_failed);
    // OpenGL Core Profile v4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    application_window.title = StringMalloc(127); // 127c + NUL
    SetStringF(true, &application_window.title, 128,
               "%s | v"__LETO__VERSION__STRING__, title);

    GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
    if (primary_monitor == NULL) ReportError(glfw_monitor_get_failed);
    application_window._m = glfwGetVideoMode(primary_monitor);
    if (application_window._m == NULL)
        ReportError(glfw_monitor_get_failed);

// Depending on what display server we're using, either set the
// Wayland app ID or the X11 class name.
#ifdef __LETO__WAYLAND__
    glfwWindowHintString(GLFW_WAYLAND_APP_ID, application_window.title);
#elif __LETO__X11__
    glfwWindowHintString(GLFW_X11_CLASS_NAME, application_window.title);
    glfwWindowHintString(GLFW_X11_INSTANCE_NAME, application_window.title);
#endif

    application_window._w = glfwCreateWindow(
        application_window._m->width, application_window._m->height,
        application_window.title, primary_monitor, NULL);
    if (application_window._w == NULL)
        ReportError(glfw_window_create_failed);

    // Make our window's OpenGL context current on this thread.
    glfwMakeContextCurrent(application_window._w);
    if (!gladLoadGL(glfwGetProcAddress)) ReportError(opengl_init_failed);
}

void DestroyWindow(void)
{
    if (application_window._w == NULL)
    {
        ReportWarning(preemptive_window_free);
        return;
    }

    StringFree(&application_window.title);
    glfwDestroyWindow(application_window._w);
    application_window._m = NULL;
    application_window._w = NULL;

    glfwTerminate();
}

bool GetApplicationRunState(void)
{
    if (application_window._w == NULL)
    {
        ReportWarning(preemptive_window_info);
        return false;
    }
    return !glfwWindowShouldClose(application_window._w);
}

void SwapApplicationBuffers(void)
{
    if (application_window._w == NULL)
    {
        ReportWarning(preemptive_buffer_swap);
        return;
    }
    glfwSwapBuffers(application_window._w);
}

const char* GetApplicationTitle(void)
{
    if (application_window.title == NULL)
        ReportWarning(preemptive_window_info);
    return application_window.title;
}

uint32_t GetApplicationWidth(void)
{
    if (application_window._m == NULL)
    {
        ReportWarning(preemptive_window_info);
        return 0;
    }
    return (uint32_t)application_window._m->width;
}

uint32_t GetApplicationHeight(void)
{
    if (application_window._m == NULL)
    {
        ReportWarning(preemptive_window_info);
        return 0;
    }
    return (uint32_t)application_window._m->height;
}
