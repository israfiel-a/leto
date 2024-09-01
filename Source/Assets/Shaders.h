/**
 * @file Shaders.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides the public interface of Leto's wrapper around OpenGL
 * shaders, such as shaders and their various helper functions.
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

// Fixed-width integers as described by the C standard.
#include <stdint.h>

/**
 * @brief A shader wrapper that contains an associated name value.
 */
typedef struct
{
    /**
     * @brief The OpenGL ID of the shader. @warning This should not be
     * changed, as doing so will more likely than not cause an OpenGL
     * error.
     */
    unsigned int id;
    /**
     * @brief The name of the shader's containing folder.
     */
    const char* name;
} shader_t;

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
shader_t* LetoLoadShader(const char* name);

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
void LetoUnloadShader(shader_t* node);

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
void LetoUseShader(const shader_t* shader);

#endif // __LETO__SHADERS___
