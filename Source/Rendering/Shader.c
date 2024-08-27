/**
 * @file Window.c
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides the implementation of the public interface defined in
 * @file Shaders.h.
 * @date 2024-08-25
 *
 * @copyright (c) 2024 - Israfiel
 * This document is under the GNU Affero General Public License v3.0. It
 * can be modified and distributed (commercially or otherwise) freely, and
 * can be used privately and within patents. No liability or warranty is
 * guaranteed. However, on use, the user must state license and copyright,
 * any changes made, and disclose the source of the document. For more
 * information see the @file LICENSE.md file included with this
 * distribution of the Leto source code.
 */

#include "Shaders.h"           // Public interface parent
#include <Output/Reporter.h>   // Error and warning reporter
#include <Utilities/Strings.h> // String utilities
#include <gl.h>                // OpenGL function pointers
#include <stdio.h>             // Standard I/O functionality
#include <stdlib.h>            // Malloc / free

static void CheckShaderCompilation(unsigned int shader)
{
    int success_flag;
    char error_info[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success_flag);
    if (!success_flag)
    {
        glGetShaderInfoLog(shader, 1024, NULL, error_info);
        printf("\nOpenGL shader error:\n%s", error_info);
        ReportError(opengl_shader_compilation_failed);
    }
}

static void CheckShaderLinkage(unsigned int shader)
{
    int success_flag;
    char error_info[1024];
    glGetProgramiv(shader, GL_COMPILE_STATUS, &success_flag);
    if (!success_flag)
    {
        glGetProgramInfoLog(shader, 1024, NULL, error_info);
        printf("\nOpenGL shader error:\n%s", error_info);
        ReportError(opengl_shader_compilation_failed);
    }
}

shader_node_t LoadShader(const char* name)
{
    char* vpath = StringCreate(256, "Assets/Shaders/%s/vertex.vs", name);
    char* fpath = StringCreate(256, "Assets/Shaders/%s/fragment.fs", name);
    FILE *vfile = fopen(vpath, "rb"), *ffile = fopen(fpath, "rb");
    if (vfile == NULL || ffile == NULL) ReportError(file_open_failed);

    if (fseek(vfile, 0L, SEEK_END) == -1 ||
        fseek(ffile, 0L, SEEK_END) == -1)
        ReportError(file_positioner_set_failed);

    long vsize = ftell(vfile) + 1, fsize = ftell(ffile) + 1;
    if (vsize == -1 || fsize == -1)
        ReportError(file_positioner_get_failed);

    if (fseek(vfile, 0L, SEEK_SET) == -1 ||
        fseek(ffile, 0L, SEEK_SET) == -1)
        ReportError(file_positioner_set_failed);

    char *vraw = malloc(vsize), *fraw = malloc(fsize);
    fread(vraw, 1, vsize - 1, vfile);
    fread(fraw, 1, fsize - 1, ffile);
    fclose(vfile), fclose(ffile);
    const char *vcode = vraw, *fcode = fraw;

    unsigned int vid = glCreateShader(GL_VERTEX_SHADER),
                 fid = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vid, 1, &vcode, NULL);
    glCompileShader(vid);
    CheckShaderCompilation(vid);

    glShaderSource(fid, 1, &fcode, NULL);
    glCompileShader(fid);
    CheckShaderCompilation(fid);

    shader_node_t created_node = SHADER_NODE_INITIALIZER;
    created_node.id = glCreateProgram();
    glAttachShader(created_node.id, vid),
        glAttachShader(created_node.id, fid);
    glLinkProgram(created_node.id);
    CheckShaderLinkage(created_node.id);

    glDeleteShader(vid), glDeleteShader(fid);
    free(vpath), free(fpath);
    free(vraw), free(fraw);

    return created_node;
}

void UnloadShader(shader_node_t* node) { glDeleteProgram(node->id); }

shader_list_t CreateShaderList(const char* initial)
{
    shader_list_t created_list = SHADER_LIST_INITIALIZER;
    created_list.length = 1;
    created_list.head = LoadShader(initial);

    return created_list;
}

void DestroyShaderList(shader_list_t* list)
{
    shader_node_t* current_node = &list->head;
    for (size_t i = 0; i < list->length - 1; i++)
    {
        UnloadShader(current_node);
        current_node = current_node->next;
    }

    list->length = 0;
    list->head = (shader_node_t)SHADER_NODE_INITIALIZER;
    list->tail = NULL;
}
