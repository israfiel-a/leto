#include "Renderer.h"
#include <Windowing/Window.h>

void render(void)
{
    while (GetApplicationRunning() == running)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        SwapApplicationBuffers();
        glfwPollEvents();
    }
}
