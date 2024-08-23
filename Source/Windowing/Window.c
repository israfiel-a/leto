#include "Window.h"
#include <Output/Reporter.h>
#include <Utilities/Strings.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static window_t application_window = {"", NULL, NULL};

static void SetGLFWContextFlags(uint8_t version_major,
                                uint8_t version_minor, uint32_t type)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, (int)version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, (int)version_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, (int)type);
}

static void CreateWindowContext(void)
{
    GLFWmonitor* primary_monitor = glfwGetPrimaryMonitor();
    if (primary_monitor == NULL) ReportError(glfw_monitor_get_failed);
    application_window._m = glfwGetVideoMode(primary_monitor);
    if (application_window._m == NULL)
        ReportError(glfw_monitor_get_failed);

#ifdef __LETO__WAYLAND__
    glfwWindowHintString(GLFW_WAYLAND_APP_ID, application_window.title);
#endif

    application_window._w = glfwCreateWindow(
        1, 1, application_window.title, primary_monitor, NULL);
    if (application_window._w == NULL)
        ReportError(glfw_window_create_failed);
    glfwMakeContextCurrent(application_window._w);
}

const window_t* CreateWindow(const char* title)
{
    if (application_window._w != NULL)
    {
        ReportWarning(double_window_creation);
        return &application_window;
    }

    bool init_success = glfwInit();
    if (!init_success) ReportError(glfw_init_failed);
    SetGLFWContextFlags(4, 6, GLFW_OPENGL_CORE_PROFILE);

    application_window.title = StringMalloc(127);
    FormattedSetString(true, &application_window.title, 128,
                       "%s | v"__LETO__VERSION__STRING__, title);

    CreateWindowContext();
    if (!gladLoadGL(glfwGetProcAddress)) ReportError(opengl_init_failed);

    glViewport(0, 0, application_window._m->width,
               application_window._m->height);
    while (!glfwWindowShouldClose(application_window._w))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

        glfwSwapBuffers(application_window._w);
        glfwPollEvents();
    }

    return &application_window;
}

void DestroyWindow(void)
{
    if (application_window._w == NULL)
    {
        ReportWarning(preemptive_window_free);
        return;
    }

    free(application_window.title);
    glfwDestroyWindow(application_window._w);
    glfwTerminate();
}
