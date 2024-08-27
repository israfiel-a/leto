/**
 * @file Shaders.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides the public interface of Leto's wrapper around OpenGL
 * shaders, such as shader linked lists and their various helper functions.
 * @date 2024-08-27
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

#ifndef __LETO__SHADERS__
#define __LETO__SHADERS__

#include <stddef.h>
#include <stdint.h>

typedef struct shader_node_t
{
    unsigned int id;
    struct shader_node_t* next;
} shader_node_t;

typedef struct
{
    shader_node_t head;
    shader_node_t* tail;
    size_t length;
} shader_list_t;

#define SHADER_NODE_INITIALIZER                                           \
    {                                                                     \
        0, NULL                                                           \
    }

#define SHADER_LIST_INITIALIZER                                           \
    {                                                                     \
        SHADER_NODE_INITIALIZER, NULL, 0                                  \
    }

shader_node_t LoadShader(const char* name);
void UnloadShader(shader_node_t* node);

shader_list_t CreateShaderList(const char* initial);
void DestroyShaderList(shader_list_t* list);

#endif // __LETO__SHADERS___
