#include "Renderer.h"
#include <Windowing/Window.h>
#include <pthread.h>
#include <stdbool.h>

pthread_mutex_t render_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t render_cond = PTHREAD_COND_INITIALIZER;
volatile bool should_render = false; // there's way better ways to do this
volatile bool should_kill = false;

void* RenderFunction(void* data)
{
    glfwMakeContextCurrent(GetApplicationWindow()->_w);

    pthread_mutex_lock(&render_mutex);
    while (!should_kill)
    {
        while (!should_render)
            pthread_cond_wait(&render_cond, &render_mutex);

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glfwSwapBuffers(GetApplicationWindow()->_w);

        pthread_cond_destroy(&render_cond);
        pthread_cond_init(&render_cond, NULL);
        should_render = false;
    }
    pthread_mutex_unlock(&render_mutex);

    return NULL;
}

void render(void)
{
    glfwMakeContextCurrent(NULL);

    pthread_t rendering_thread;
    int thread_create_code =
        pthread_create(&rendering_thread, NULL, RenderFunction, NULL);
    if (thread_create_code != 0)
    {
        // handle thread errors here (man pthread_create)
        return;
    }

    while (GetApplicationRunning() == running)
    {
        should_render = true;
        pthread_cond_signal(&render_cond);
        glfwPollEvents();
    }
    should_kill = true;
    pthread_cond_signal(&render_cond);
}
