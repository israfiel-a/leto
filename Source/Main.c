#include <Rendering/Renderer.h>
#include <Rendering/Window.h>

int main(int argc, char** argv)
{
    CreateWindow("Leto");
    render();
    DestroyWindow();
}
