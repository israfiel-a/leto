#include <interface/renderer.h>
#include <interface/window.h>

int main(void)
{
    LetoCreateWindow("Leto");
    LetoCreateRenderer(1);
    LetoAddShader("basic");

    render();

    LetoDestroyRenderer();
    LetoDestroyWindow();
}
