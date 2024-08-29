#include "Renderer.h"
#include "Shaders.h"
#include "Window.h"
#include <gl.h>
#include <glfw3.h>

typedef struct
{
    shader_list_t shader_list;
} renderer_t;

static renderer_t application_renderer = {SHADER_LIST_INITIALIZER};

void CreateRenderer(void)
{
    application_renderer.shader_list = CreateShaderList("basic");
}

void DestroyRenderer(void)
{
    DestroyShaderList(&application_renderer.shader_list);
}

void render(void)
{
    UseShaderN(&application_renderer.shader_list, "basic");

    while (GetApplicationRunState())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        SwapApplicationBuffers();
        glfwPollEvents();
    }
}
