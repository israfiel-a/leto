#include <Output/Renderer.h>
#include <Windowing/Window.h>

int main(int argc, char** argv)
{
    CreateWindow("Leto");
    render();
    DestroyWindow();
}
