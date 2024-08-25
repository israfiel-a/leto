/**
 * @file Window.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides Leto's windowing interface, which is an abstraction over
 * the GLFW library. All explanations, errors, and warnings can be found
 * documented with their parent object.
 * @date 2024-08-22
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef __LETO__WINDOW__
#define __LETO__WINDOW__

// The GLAD2 OpenGL header file. Provides initialization functions and
// rendering logic.
#include <gl.h>
// The GLFW windowing library's header file.
#include <glfw3.h>

/**
 * @brief The structure that contains all information to be associated with
 * our application's window, like the @struct GLFWwindow parent object, the
 * window's title, and the user's primary monitor's information.
 */
typedef struct
{
    /**
     * @brief The title of the window. This value is @ref malloc ed by @ref
     * CreateWindow, and @ref free d by @ref DestroyWindow. @note This
     * value is included purely for record purposes. Any changes to this
     * value wil @b not be pushed to the actual window.
     */
    char* title;
    /**
     * @brief The internal GLFW object containing our window's state;
     * width, height, title, contexts, etc. @warning This value should not
     * be edited outside of helper functions, as doing so may cause memory
     * leaks or crashes.
     */
    GLFWwindow* _w;
    /**
     * @brief This value represents the default monitor's video
     * information. This includes values like the height and width of the
     * monitor, the refresh rate, and more.
     */
    const GLFWvidmode* _m;
} window_t;

/**
 * @brief An enumerator representing the various stages of "running" a
 * window can have.
 */
typedef enum
{
    /**
     * @brief The window is currently running.
     */
    running,
    /**
     * @brief The window is currently in the process of closing.
     */
    closing,
    /**
     * @brief The window has closed.
     */
    closed
} window_close_state_t;

/**
 * DESCRIPTION
 *
 * @brief Allocate space for and initialize the application's window
 * object. This will initialize both the GLAD2 and GLFW libraries along
 * with the window. To prevent unneeded memory bloat, calling this function
 * twice will both result in a warning and it will return the exact same
 * value that was returned the first time.
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
 * @return A pointer to the application window. This is returned simply for
 * record-keeping purposes, and as such the pointer is constant.
 *
 * WARNINGS
 *
 * Only two warnings can be raised from this function.
 * @warning double_window_creation -- If this function is called twice,
 * this warning will be reported, and the application window created before
 * will be returned with no changes.
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
const window_t* CreateWindow(const char* title);

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
 * @brief Get a reference to the application window.
 *
 * PARAMETERS
 *
 * Nothing of note.
 *
 * RETURN VALUE
 *
 * @return A constant pointer to the application window. The pointer is
 * marked constant to prevent accidental edits that could crash the
 * program.
 *
 * WARNINGS
 *
 * Nothing of note.
 *
 * ERRORS
 *
 * Nothing of note.
 *
 */
const window_t* GetApplicationWindow(void);

/**
 * DESCRIPTION
 *
 * @brief Get the running state of the application window. If given
 * "closed", the application should probably already have quit, if returned
 * "closing" the application should start (or have already started)
 * cleaning up its data, and if given "running", just continue as usual.
 *
 * PARAMETERS
 *
 * Nothing of note.
 *
 * RETURN VALUE
 *
 * @return The close state of the window, as described by @enum
 * window_close_state_t.
 *
 * WARNINGS
 *
 * Nothing of note.
 *
 * ERRORS
 *
 * Nothing of note.
 *
 */
window_close_state_t GetApplicationRunning(void);

void SwapApplicationBuffers(void);

#endif // __LETO__WINDOW__
