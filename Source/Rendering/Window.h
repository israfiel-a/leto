/**
 * @file Window.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides Leto's windowing interface, which is an abstraction over
 * the GLFW library. All explanations, errors, and warnings can be found
 * documented with their parent object.
 * @date 2024-08-22
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

#ifndef __LETO__WINDOW__
#define __LETO__WINDOW__

// The standard boolean type as described by the C11 standard.
#include <stdbool.h>
// Fixed-width integer types as described by the C11 standard.
#include <stdint.h>

/**
 * DESCRIPTION
 *
 * @brief Allocate space for and initialize the application's window
 * object. This will initialize both the GLAD2 and GLFW libraries along
 * with the window. To prevent unneeded memory bloat, calling this function
 * twice will result in a warning and instant return.
 *
 * PARAMETERS
 *
 * @param title A string literal specifying what the base title of the
 * window should be. @note This will not be the application window's title
 * verbatim, as the function appends " | v[LETO_VERSION]" to the end of
 * this literal.
 *
 * RETURN VALUE
 *
 * Nothing of note.
 *
 * WARNINGS
 *
 * Only two warnings can be raised from this function.
 * @warning double_window_creation -- If this function is called twice,
 * this warning will be reported, and the application window created before
 * will not be affected.
 * @warning string_overconcat -- If the title of the window is too large to
 * fit within 127 characters when mashed together with the version, this
 * warning will be thrown. This also means your window's title will likely
 * be mangled.
 *
 * ERRORS
 *
 * Four errors can be thrown by this function, with five different possible
 * points of error.
 * @exception glfw_init_failed -- If the initialization of the GLFW library
 * does not work, this error will be thrown, and the process will be
 * closed.
 * @exception opengl_init_failed -- If the initialization of the GLAD2
 * library fails, this error will be thrown, and the process will exit.
 * @exception glfw_monitor_get_failed -- This error can be thrown twice,
 * first if GLFW fails to find the user's primary monitor, and second if we
 * cannot get the video data from said monitor.
 * @exception glfw_window_create_failed -- If the creation of the GLFW
 * window fails, this error will be thrown, and the process will exit.
 *
 */
void CreateWindow(const char* title);

/**
 * DESCRIPTION
 *
 * @brief Destroy and de-initialize all objects having to do with the
 * application's window and OpenGL contexts. This should be one of the
 * final function calls within the application's runtime.
 *
 * PARAMETERS
 *
 * Nothing of note.
 *
 * RETURN VALUE
 *
 * Nothing of note.
 *
 * WARNINGS
 *
 * Only a single warning can be triggered in this function.
 * @warning preemptive_window_free -- If this function is called twice, or
 * before @ref CreateWindow is called, this warning will be thrown.
 *
 * ERRORS
 *
 * Nothing of note.
 *
 */
void DestroyWindow(void);

/**
 * DESCRIPTION
 *
 * @brief Swap the back and front buffers of the application window. This
 * functionality is to prevent screen tearing and visual artifacts from
 * half-created frames being rendered. This function should not be called
 * before @ref CreateWindow.
 *
 * PARAMETERS
 *
 * Nothing of note.
 *
 * RETURN VALUE
 *
 * Nothing of note.
 *
 * WARNINGS
 *
 * One warning can be thrown by this function.
 * @warning preemptive_buffer_swap -- If the application window is NULL,
 * e.g. if the @ref CreateWindow function hasn't been called yet, this
 * warning will be thrown and nothing will be done.
 *
 * ERRORS
 *
 * Nothing of note.
 *
 */
void SwapApplicationBuffers(void);

/**
 * DESCRIPTION
 *
 * @brief Get the running state of the application window. If given false,
 * the window is in the process of closing and all resources should be
 * cleaned up. Else, keep doing whatever was happening before hand.
 *
 * PARAMETERS
 *
 * Nothing of note.
 *
 * RETURN VALUE
 *
 * @return The close state of the window.
 *
 * WARNINGS
 *
 * One warning can be thrown by this function.
 * @warning preemptive_window_info -- This warning is triggered should this
 * function be called before the window is initialized. False is
 * automatically returned.
 *
 * ERRORS
 *
 * Nothing of note.
 *
 */
bool GetApplicationRunState(void);

/**
 * DESCRIPTION
 *
 * @brief Get the title of the application window. This string is
 * constructed by @ref CreateWindow.
 *
 * PARAMETERS
 *
 * Nothing of note.
 *
 * RETURN VALUE
 *
 * Nothing of note.
 *
 * WARNINGS
 *
 * One warning can be thrown by this function.
 * @warning preemptive_window_info -- If the window has not yet been
 * created, this warning will be thrown, and NULL will be returned.
 *
 * ERRORS
 *
 * Nothing of note.
 *
 */
const char* GetApplicationTitle(void);

/**
 * DESCRIPTION
 *
 * @brief Get the width of the application window.
 *
 * PARAMETERS
 *
 * Nothing of note.
 *
 * RETURN VALUE
 *
 * @return The width of the window in screen coordinates. This value is
 * converted from GLFW's internally-utilized @code int @endcode type to a
 * more appropriate @code uint32_t @endcode.
 *
 * WARNINGS
 *
 * One warning can be thrown by this function.
 * @warning preemptive_window_info -- If the window has not yet been
 * created, this warning will be thrown, and 0 will be returned.
 *
 * ERRORS
 *
 * Nothing of note.
 *
 */
uint32_t GetApplicationWidth(void);

/**
 * DESCRIPTION
 *
 * @brief Get the height of the application window.
 *
 * PARAMETERS
 *
 * Nothing of note.
 *
 * RETURN VALUE
 *
 * @return The height of the window in screen coordinates. This value is
 * converted from GLFW's internally-utilized @code int @endcode type to a
 * more appropriate @code uint32_t @endcode.
 *
 * WARNINGS
 *
 * One warning can be thrown by this function.
 * @warning preemptive_window_info -- If the window has not yet been
 * created, this warning will be thrown, and 0 will be returned.
 *
 * ERRORS
 *
 * Nothing of note.
 *
 */
uint32_t GetApplicationHeight(void);

#endif // __LETO__WINDOW__
