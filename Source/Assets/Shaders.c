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
static void CheckShaderCompilation_(const unsigned int shader)
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
        LetoReportError(opengl_shader_compilation_failed);
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
static void CheckShaderLinkage_(const unsigned int shader)
{
    int success_flag = false;
    char error_info[1024];

    glGetProgramiv(shader, GL_LINK_STATUS, &success_flag);
    if (!success_flag)
    {
        glGetProgramInfoLog(shader, 1024, NULL, error_info);
        printf("\nOpenGL shader link error:\n%s", error_info);
        LetoReportError(opengl_shader_compilation_failed);
    }
}

shader_t* LetoLoadShader(const char* name)
{
    if (name == NULL)
    {
        LetoReportWarning(null_string);
        return NULL;
    }

    char* vraw =
        LetoGetFileContentsPF("Assets/Shaders/%s/vertex.vs", name);
    char* fraw =
        LetoGetFileContentsPF("Assets/Shaders/%s/fragment.fs", name);
    const char *vcode = vraw, *fcode = fraw;

    unsigned int vid = glCreateShader(GL_VERTEX_SHADER),
                 fid = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vid, 1, &vcode, NULL);
    glCompileShader(vid);
    CheckShaderCompilation_(vid);

    glShaderSource(fid, 1, &fcode, NULL);
    glCompileShader(fid);
    CheckShaderCompilation_(fid);

    free(vraw), free(fraw);

    shader_t* created_node = calloc(sizeof(shader_t), 1);
    if (created_node == NULL) LetoReportError(failed_allocation);
    created_node->name = name;

    created_node->id = glCreateProgram();
    glAttachShader(created_node->id, vid);
    glAttachShader(created_node->id, fid);
    glLinkProgram(created_node->id);
    CheckShaderLinkage_(created_node->id);

    glDeleteShader(vid), glDeleteShader(fid);
    return created_node;
}

void LetoUnloadShader(shader_t* node)
{
    if (node == NULL)
    {
        LetoReportWarning(null_object);
        return;
    }

    glDeleteProgram(node->id);
    free(node);
}

void LetoUseShader(const shader_t* shader)
{
    if (shader == NULL)
    {
        LetoReportWarning(null_object);
        return;
    }

    glUseProgram(shader->id);
    if (glGetError() != GL_NO_ERROR)
        LetoReportError(opengl_malformed_shader);
}
