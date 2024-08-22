#include "Window.h"
#include "Output/Reporter.h"
#include <stdbool.h>
#include <string.h>

window_t application_window = {"", NULL};

const window_t* CreateWindow(const char* title)
{
    if (application_window._w != NULL)
    {
        ReportWarning(double_window_creation);
        return &application_window;
    }

    bool init_success = glfwInit();
    if (!init_success) ReportError(glfw_init_failed);

    // OpenGL Core v4.6
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    strncat(application_window.title, title, 64);
    strncat(application_window.title, " | v" __LETO__VERSION__STRING__,
            64);

    application_window._w =
        glfwCreateWindow(640, 480, application_window.title, NULL, NULL);
    if (application_window._w == NULL)
        ReportError(glfw_window_create_failed);

    glfwMakeContextCurrent(application_window._w);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        ReportError(opengl_init_failed);

    while (!glfwWindowShouldClose(application_window._w))
    {
        glClear(GL_COLOR_BUFFER_BIT);

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

    glfwMakeContextCurrent(NULL);
    glfwDestroyWindow(application_window._w);
    glfwTerminate();
}
