#include "Window.h"
#include <Diagnostic/Platform.h> // Platform macros
#include <Output/Reporter.h>     // Warning and error reporting
#include <Utilities/Strings.h>   // String utilities

/**
 * @brief This is the application window's main structure. It is stored
 * exclusively in this file, and all references that leak from this file
 * are constant. The will be initialized by @ref CreateWindowContext and
 * returned back to NULL via @ref DestroyWindow.
 */
static window_t application_window = {NULL, NULL, NULL};

const window_t* CreateWindow(const char* title)
{
    if (application_window._w != NULL)
    {
        ReportWarning(double_window_creation);
        return &application_window;
    }

    if (!glfwInit()) ReportError(glfw_init_failed);
    // OpenGL Core Profile v4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    application_window.title = StringMalloc(127); // 127c + NUL
    FormattedSetString(true, &application_window.title, 128,
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

    return &application_window;
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

const window_t* GetApplicationWindow(void) { return &application_window; }

window_close_state_t GetApplicationRunning(void)
{
    if (application_window._w == NULL) return closed;
    if (glfwWindowShouldClose(application_window._w)) return closing;
    return running;
}
