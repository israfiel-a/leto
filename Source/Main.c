#include <Rendering/Renderer.h>
#include <Rendering/Window.h>

int main(void)
{
    LetoCreateWindow("Leto");
    LetoCreateRenderer(1);
    LetoAddShader("basic");

    render();

    LetoDestroyRenderer();
    LetoDestroyWindow();
}
