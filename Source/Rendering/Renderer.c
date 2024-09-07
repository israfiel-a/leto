#include "Renderer.h"
#include "Window.h"
#include <Output/Reporter.h>
#include <gl.h>
#include <glfw3.h>
#include <stdlib.h>
#include <string.h>

static renderer_t application_renderer = {NULL, 0, 0};

void LetoCreateRenderer(size_t shader_list_size)
{
    application_renderer.shader_list =
        malloc(sizeof(shader_t) * shader_list_size);
    application_renderer.shader_list_size = shader_list_size;
    application_renderer.shader_list_occupied = 0;
}

void LetoDestroyRenderer(void)
{
    for (size_t i = 0; i < application_renderer.shader_list_occupied; i++)
        LetoUnloadShader(application_renderer.shader_list[i]);
    free(application_renderer.shader_list);
}

void render(void)
{
    LetoUseShader(LetoGetShader("basic"));

    while (LetoGetRunState())
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        LetoSwapBuffers();
        glfwPollEvents();
    }
}

shader_t* LetoGetShader(const char* name)
{
    if (name == NULL)
    {
        LetoReport(null_param);
        return application_renderer.shader_list[0];
    }

    shader_t* current_shader = application_renderer.shader_list[0];
    for (size_t i = 0; i < application_renderer.shader_list_size; i++)
    {
        if (current_shader == NULL) break;
        if (strcmp(current_shader->name, name) == 0) return current_shader;
        current_shader = application_renderer.shader_list[i];
    }

    LetoReport(no_such_value);
    return NULL;
}

void LetoAddShader(const char* name)
{
    if (application_renderer.shader_list_size ==
        application_renderer.shader_list_occupied)
    {
        LetoReport(array_full);
        return;
    }

    if (name == NULL)
    {
        LetoReport(null_param);
        return;
    }

    application_renderer
        .shader_list[application_renderer.shader_list_occupied] =
        LetoLoadShader(name);
    application_renderer.shader_list_occupied += 1;
}
