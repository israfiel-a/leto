#include "Meshes.h"
#include <stdio.h>

mesh_t* LetoLoadMesh(const char* name)
{
    printf("%s\n", name);
    return NULL;
}
void LetoUnloadMesh(mesh_t* mesh)
{
    printf("unloaded, %s\n", mesh->name);
    return;
}
