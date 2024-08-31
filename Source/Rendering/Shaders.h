/**
 * @file Shaders.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides the public interface of Leto's wrapper around OpenGL
 * shaders, such as shader linked lists and their various helper functions.
 * @date 2024-08-27
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

#ifndef __LETO__SHADERS__
#define __LETO__SHADERS__

// Definitions provided by the C standard.
#include <stddef.h>
// Fixed-width integers as described by the C standard.
#include <stdint.h>

/**
 * @brief A single node within a singly-linked list of shaders. This
 * contains an ID, name, and the next element of the list.
 */
typedef struct shader_node_t
{
    /**
     * @brief The name of the shader's containing folder. This is used
     * mainly to find the shader when searching through a list.
     */
    const char* name;
    /**
     * @brief The OpenGL ID of the shader. @warning This should not be
     * changed, as doing so will more likely than not cause an OpenGL
     * error.
     */
    unsigned int id;
    /**
     * @brief A pointer to the next node in the list.
     */
    struct shader_node_t* next;
} shader_node_t;

/**
 * @brief A full list of shaders. This contains a list of shaders and the
 * length of the list.
 */
typedef struct
{
    /**
     * @brief A pointer to the head of the shader list. This links to the
     * rest of the list.
     */
    shader_node_t* head;
    /**
     * @brief A pointer to the tail end of the list. This value can also be
     * reached by iterating through the list.
     */
    shader_node_t* tail;
    /**
     * @brief The length of the linked list.
     */
    size_t length;
} shader_list_t;

/**
 * @brief An initializer for a shader list. This is basically a NULL
 * version of a shader list.
 */
#define SHADER_LIST_INITIALIZER                                           \
    {                                                                     \
        NULL, NULL, 0                                                     \
    }

/**
 * DESCRIPTION
 *
 * @brief Load a shader from its text file(s) into the program's memory.
 * This file will automatically detect any and all files within the given
 * directory, and uses file extensions to figure out what capabilities the
 * shader has (compute shader, etc.).
 * @todo Implement shader type decision and support for more than just
 * vertex and fragment shaders.
 *
 * PARAMETERS
 *
 * @param name The name of the folder in which this shader resides. This
 * string is @b NOT santized. It is up to the caller to make certain any
 * paths passed to this function are not malformed.
 *
 * RETURN VALUE
 *
 * @return A dynamically allocated shader node, containing the OpenGL ID of
 * the shader created. To free this value, utilize @ref UnloadShader.
 *
 * WARNINGS
 *
 * Only one warning is called directly by this function.
 * @warning null_string -- If the @param name value passed to the function
 * is NULL, this warning will be thrown and nothing will be done. The
 * function returns NULL.
 * @note For possible warnings unhandled by this function, see @ref
 * GetFileContentsPF.
 *
 * ERRORS
 *
 * Only two errors can be thrown directly by this function.
 * @exception failed_allocation -- If the reservation of space for the new
 * node somehow goes awry, this error is thrown, and the process quits.
 * @exception opengl_shader_compilation_failed -- This error is a little
 * bit weird, as it doesn't print once; it prints twice. Because of the
 * semantics of @ref glGetShaderInfoLog, it must be called locally, but
 * Leto's error interface does not allow for this. To rectify this, we send
 * a message beforehand detailing the specific error that occurred. @note
 * This error is thrown for compilation @b and linking errors, but the
 * context provided beforehand differentiates the two.
 * @note For possible errors unhandled by this function, see @ref
 * GetFileContentsPF.
 *
 */
shader_node_t* LoadShader(const char* name);

/**
 * DESCRIPTION
 *
 * @brief Unload a shader from memory, and free all information associated
 * with it. This function is basically a wrapper around two @ref free
 * calls.
 *
 * PARAMETERS
 *
 * @param node The shader to unload.
 *
 * RETURN VALUE
 *
 * Nothing to note.
 *
 * WARNINGS
 *
 * Only one warning can be thrown by this function.
 * @warning null_object -- If the passed @param node value is NULL, this
 * error will be thrown, and nothing will be done.
 *
 * ERRORS
 *
 * Nothing to note.
 *
 */
void UnloadShader(shader_node_t* node);

/**
 * DESCRIPTION
 *
 * @brief Create a linked list of shaders, and load its initial member into
 * memory.
 *
 * PARAMETERS
 *
 * @param initial The folder name of the first shader to be loaded. This
 * value is @b not santitized, so it's up to the caller to sanitize the
 * value passed to this function.
 *
 * RETURN VALUE
 *
 * @return The newly created shader list.
 *
 * WARNINGS
 *
 * Nothing to note.
 * @note For possible warnings unhandled by this function, see @ref
 * LoadShader.
 *
 * ERRORS
 *
 * Nothing to note.
 *
 */
shader_list_t CreateShaderList(const char* initial);

/**
 * DESCRIPTION
 *
 * @brief Free all memory associated with a given shader list, and set all
 * members to NULL.
 *
 * PARAMETERS
 *
 * @param list The list to free.
 *
 * RETURN VALUE
 *
 * Nothing to note.
 *
 * WARNINGS
 *
 * One warning can be thrown by this function.
 * @warning null_object -- This warning can potentially be thrown twice.
 * First; if the list passed to the function is NULL, this warning will be
 * thrown. Second; if the head element of the given list is NULL, this
 * warning will be thrown inside @ref UnloadShader.
 *
 * ERRORS
 *
 * Nothing to note.
 *
 */
void DestroyShaderList(shader_list_t* list);

/**
 * DESCRIPTION
 *
 * @brief Add a shader node to the specified list. This recalculates all
 * elements of the shader array as well, such as length and tail.
 *
 * PARAMETERS
 *
 * @param list The list to add to.
 * @param name The name of the folder that contains the given shader's text
 * files.
 *
 * RETURN VALUE
 *
 * Nothing to note.
 *
 * WARNINGS
 *
 * Two warnings can be thrown by this function.
 * @warning null_string -- If the @param name passed to this function is
 * NULL, this warning will be thrown and nothing will be done.
 * @warning null_object -- If the list passed to the function is NULL, this
 * warning will be thrown and nothing will be done.
 * @note For possible warnings unhandled by this function, see @ref
 * LoadShader.
 *
 * ERRORS
 *
 * Nothing to note.
 * @note For possible errors unhandled by this function, see @ref
 * LoadShader.
 *
 */
void AddShaderToList(shader_list_t* list, const char* name);

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
 * @return The shader found, or NULL if none could be found or something
 * else went wrong.
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
uint32_t GetShader(shader_list_t* list, const char* name);

/**
 * DESCRIPTION
 *
 * @brief Grab the first shader node whose name matches the one provided.
 *
 * PARAMETERS
 *
 * @param list The list to search through.
 * @param name The name to search nodes for.
 *
 * RETURN VALUE
 *
 * @return The shader found, or NULL if none could be found or something
 * else went wrong.
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
shader_node_t* GetShaderN(shader_list_t* list, const char* name);

/**
 * DESCRIPTION
 *
 * @brief Use an OpenGL shader via its containing shader node.
 *
 * PARAMETERS
 *
 * @param shader The shader to use.
 *
 * RETURN VALUE
 *
 * Nothing to note.
 *
 * WARNINGS
 *
 * One warning can be thrown by this function.
 * @warning null_object -- If the shader passed to this function is NULL,
 * then this warning is thrown and nothing happens.
 *
 * ERRORS
 *
 * One error can be thrown by this function.
 * @exception opengl_malformed_shader - If the shader inside the node is
 * not valid, this error is thrown and the process exits.
 *
 */
void UseShader(shader_node_t* shader);

/**
 * DESCRIPTION
 *
 * @brief Use an OpenGL shader via looking it up from a list.
 *
 * PARAMETERS
 *
 * @param list The list to look through.
 *
 * RETURN VALUE
 *
 * Nothing to note.
 *
 * WARNINGS
 *
 * Three warnings can be thrown by this function.
 * @warning null_object -- If the list passed to this function is NULL,
 * then this warning is thrown and nothing happens.
 * @warning null_string -- If the string name of the shader passed to this
 * function is NULL, then this warning is thrown nothing happens.
 * @warning no_such_shader -- If the shader we're asking for does not
 * exist, this warning is thrown and nothing happens.
 *
 * ERRORS
 *
 * One error can be thrown by this function.
 * @exception opengl_malformed_shader - If the shader inside the node is
 * not valid, this error is thrown and the process exits.
 *
 */
void UseShaderN(shader_list_t* list, const char* name);

#endif // __LETO__SHADERS___
