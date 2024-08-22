#include "Window.h"
#include "Output/Reporter.h"
#include <stdbool.h>
#include <string.h>

static window_t application_window = {"", NULL};

const window_t* CreateWindow(const char* title)
{
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

    glfwMakeContextCurrent(application_window._w);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        ReportError(opengl_init_failed);

    while (!glfwWindowShouldClose(application_window._w))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(application_window._w);
        glfwPollEvents();
    }

    glfwTerminate();

    return &application_window;
}
