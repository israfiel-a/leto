/**
 * @file Renderer.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides Leto's rendering interface, which handles the creation
 * of secondary rendering threads, context copying, most OpenGL things, and
 * more.
 * @date 2024-08-25
 *
 * @copyright (c) 2024 - the Leto Team
 */

#ifndef __LETO__RENDERER__
#define __LETO__RENDERER__

#include <Assets/Shaders.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct
{
    shader_t** shader_list;
    size_t shader_list_size;
    size_t shader_list_occupied;
} renderer_t;

void CreateRenderer(size_t shader_list_size);
void DestroyRenderer(void);

/**
 * DESCRIPTION
 *
 * @brief Grab the inner ID of the first shader whose name matches the one
 * provided.
 *
 * PARAMETERS
 *
 * @param list The list to search through.
 * @param name The name to search nodes for.
 *
 * RETURN VALUE
 *
 * @return A pointer to the shader found, or NULL if none could be found or
 * something else went wrong.
 *
 * WARNINGS
 *
 * Three warnings can be thrown by this function.
 * @warning null_object -- If the list passed to the function is NULL, this
 * warning will be thrown and NULL is returned.
 * @warning null_string -- If the name passed to this function is NULL,
 * this warning will be thrown and the list head is returned.
 * @warning no_such_shader -- If no shader was found with the name @param
 * name, this warning is thrown and NULL is returned.
 *
 * ERRORS
 *
 * Nothing to note.
 *
 */
shader_t* GetShader(const char* name);

void AddShader(const char* name);

//! temp
void render(void);

#endif // __LETO__RENDERER__
