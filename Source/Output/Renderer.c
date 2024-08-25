#include "Renderer.h"
#include <Windowing/Window.h>

void render(void)
{
    glViewport(0, 0, GetApplicationWindow()->_m->width,
               GetApplicationWindow()->_m->height);
    while (GetApplicationRunning() == running)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

        glfwSwapBuffers(GetApplicationWindow()->_w);
        glfwPollEvents();
    }
}
