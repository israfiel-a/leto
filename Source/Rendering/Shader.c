/**
 * @file Window.c
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides the implementation of the public interface defined in
 * @file Shaders.h.
 * @date 2024-08-25
 *
 * @copyright (c) 2024 - the Leto Team
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
#include <Utilities/Files.h>   // File utilities
#include <Utilities/Strings.h> // String utilities
#include <gl.h>                // OpenGL function pointers
#include <stdio.h>             // Standard I/O functionality
#include <stdlib.h>            // Malloc / free

/**
 * DESCRIPTION
 *
 * @brief Check for any compilation errors having to do with an individual
 * OpenGL shader. If you're looking for errors relating to shader linking,
 * look at @ref CheckShaderLinkage.
 *
 * PARAMETERS
 *
 * @param shader The ID of the shader whose compilation we're meant to be
 * checking. This is marked constant to reflect its immutable state within
 * the function.
 *
 * RETURN VALUE
 *
 * Nothing of note.
 *
 * WARNINGS
 *
 * Nothing of note.
 *
 * ERRORS
 *
 * A single error can be thrown by this function.
 * @exception opengl_shader_compilation_failed -- This error is a little
 * bit weird, as it doesn't print once; it prints twice. Because of the
 * semantics of @ref glGetShaderInfoLog, it must be called locally, but
 * Leto's error interface does not allow for this. To rectify this, we send
 * a message before hand detailing the specific error that occurred.
 *
 */
static void CheckShaderCompilation(const unsigned int shader)
{
    int success_flag = true;
    char error_info[1024];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success_flag);
    if (!success_flag)
    {
        glGetShaderInfoLog(shader, 1024, NULL, error_info);
        // We don't care about any printing errors at this point, we're
        // killing the process anyway.
        (void)printf("\nOpenGL shader comp error:\n%s", error_info);
        ReportError(opengl_shader_compilation_failed);
    }
}

/**
 * DESCRIPTION
 *
 * @brief Check for any link errors having to do with an OpenGL shader
 * program. If you're looking for errors relating to shader compilation,
 * look at @ref CheckShaderCompilation.
 *
 * PARAMETERS
 *
 * @param shader The ID of the shader whose compilation we're meant to be
 * checking. This is marked constant to reflect its immutable state within
 * the function.
 *
 * RETURN VALUE
 *
 * Nothing of note.
 *
 * WARNINGS
 *
 * Nothing of note.
 *
 * ERRORS
 *
 * A single error can be thrown by this function.
 * @exception opengl_shader_compilation_failed -- This error is a little
 * bit weird, as it doesn't print once; it prints twice. Because of the
 * semantics of @ref glGetShaderInfoLog, it must be called locally, but
 * Leto's error interface does not allow for this. To rectify this, we send
 * a message before hand detailing the specific error that occurred. @note
 * This error is the same as for compilation errors, but the context
 * provided beforehand differentiates the two.
 *
 */
static void CheckShaderLinkage(const unsigned int shader)
{
    int success_flag = false;
    char error_info[1024];

    glGetProgramiv(shader, GL_LINK_STATUS, &success_flag);
    if (!success_flag)
    {
        glGetProgramInfoLog(shader, 1024, NULL, error_info);
        printf("\nOpenGL shader link error:\n%s", error_info);
        ReportError(opengl_shader_compilation_failed);
    }
}

shader_node_t* LoadShader(const char* name)
{
    if (name == NULL)
    {
        ReportWarning(null_string);
        return NULL;
    }

    char* vraw = GetFileContentsPF("Assets/Shaders/%s/vertex.vs", name);
    char* fraw = GetFileContentsPF("Assets/Shaders/%s/fragment.fs", name);
    const char *vcode = vraw, *fcode = fraw;

    unsigned int vid = glCreateShader(GL_VERTEX_SHADER),
                 fid = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vid, 1, &vcode, NULL);
    glCompileShader(vid);
    CheckShaderCompilation(vid);

    glShaderSource(fid, 1, &fcode, NULL);
    glCompileShader(fid);
    CheckShaderCompilation(fid);

    free(vraw), free(fraw);

    shader_node_t* created_node = calloc(sizeof(shader_node_t), 1);
    if (created_node == NULL) ReportError(failed_allocation);
    created_node->name = name;

    created_node->id = glCreateProgram();
    glAttachShader(created_node->id, vid);
    glAttachShader(created_node->id, fid);
    glLinkProgram(created_node->id);
    CheckShaderLinkage(created_node->id);

    glDeleteShader(vid), glDeleteShader(fid);
    return created_node;
}

void UnloadShader(shader_node_t* node)
{
    if (node == NULL)
    {
        ReportWarning(null_object);
        return;
    }

    glDeleteProgram(node->id);
    free(node);
}

shader_list_t CreateShaderList(const char* initial)
{
    // We don't check for NULLs here since LoadShader does that for us, and
    // it will simply return NULL, preventing any side effects if the
    // function return is checked correctly.

    shader_list_t created_list = SHADER_LIST_INITIALIZER;
    created_list.length = 1;
    created_list.head = LoadShader(initial);

    return created_list;
}

void DestroyShaderList(shader_list_t* list)
{
    if (list == NULL)
    {
        ReportWarning(null_object);
        return;
    }

    shader_node_t* current_node = list->head;
    for (size_t i = 0; i < list->length; i++)
    {
        shader_node_t* temp_node = current_node->next;
        UnloadShader(current_node);
        current_node = temp_node;
    }

    list->length = 0;
    list->head = NULL;
    list->tail = NULL;
}

void AddShaderToList(shader_list_t* list, const char* name)
{
    if (name == NULL)
    {
        ReportWarning(null_string);
        return;
    }

    if (list == NULL)
    {
        ReportWarning(null_object);
        return;
    }

    list->length = list->length + 1;
    list->tail->next = LoadShader(name);
    list->tail = list->tail->next;
}

shader_node_t* GetShaderNode(shader_list_t* list, const char* name)
{
    if (list == NULL)
    {
        ReportWarning(null_object);
        return NULL;
    }

    if (name == NULL)
    {
        ReportWarning(null_string);
        return list->head;
    }

    if (strcmp(list->head->name, name) == 0) return list->head;
    if (strcmp(list->tail->name, name) == 0) return list->tail;

    shader_node_t* current_node = list->head->next;
    for (size_t i = 0; i < list->length; i++)
    {
        if (current_node == NULL) break;
        if (strcmp(current_node->name, name) == 0) return current_node;
        current_node = current_node->next;
    }

    ReportWarning(no_such_shader);
    return NULL;
}

void UseShader(shader_node_t* shader)
{
    if (shader == NULL)
    {
        ReportWarning(null_object);
        return;
    }

    glUseProgram(shader->id);
    if (glGetError() != GL_NO_ERROR) ReportError(opengl_malformed_shader);
}

void UseShaderN(shader_list_t* list, const char* name)
{
    if (list == NULL)
    {
        ReportWarning(null_object);
        return;
    }

    if (name == NULL)
    {
        ReportWarning(null_string);
        return;
    }

    shader_node_t* shader = GetShaderNode(list, name);
    if (shader == NULL) return; // The warning is already reported.

    glUseProgram(shader->id);
    if (glGetError() != GL_NO_ERROR) ReportError(opengl_malformed_shader);
}
