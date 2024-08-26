#include "Renderer.h"
#include "Window.h"
#include <gl.h>
#include <glfw3.h>

void render(void)
{
    while (GetApplicationRunState())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        SwapApplicationBuffers();
        glfwPollEvents();
    }
}
