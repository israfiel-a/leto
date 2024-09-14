#include "meshes.h"
#include <io/files.h>
#include <stdio.h>
#include <utilities/strings.h>

mesh_t* LetoLoadMesh(const char* name)
{
    char* obj_file_contents =
        (char*)LetoReadFilePV(true, ASSET_DIR "/Meshes/%s", name);
    char** mesh_data = LetoStringSplit(obj_file_contents, '\n');
    // LetoStringFree(&obj_file_contents);

    for (size_t i = 0; mesh_data[i] != NULL; i++)
        printf("%s\n\n", mesh_data[i]);

    return NULL;
}
void LetoUnloadMesh(mesh_t* mesh)
{
    printf("unloaded, %s\n", mesh->name);
    return;
}
