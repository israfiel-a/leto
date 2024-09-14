/**
 * @file Reporter.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides a standard interface for throwing and handling errors,
 * warnings, and all of their peripherals.
 * @date 2024-09-07
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

#ifndef __LETO__REPORTER__
#define __LETO__REPORTER__

// Boolean definitions as provided by the C standard.
#include <stdbool.h>
// Fixed-width integers as provided by the C standard.
#include <stdint.h>

/**
 * @brief An enumerator describing each and every problem that could
 * possibly occur within a Leto session.
 */
typedef enum
{
    /**
     * @defgroup General-purpose problems.
     */
    null_param,
    failed_buffer,
    small_buffer,
    no_such_value,
    array_full,
    string_error,
    time_error,
    /**
     * @defgroup Specific interface problems.
     */
    glfw_init,
    glfw_monitor,
    gl_init,
    gl_shader_comp,
    gl_shader_bad,
    null_window, // failed window
    window_null, // window not yet created
    file_null,   // file not yet created
    file_pos_get,
    file_pos_set,
    file_read, // sometimes failed file open
    file_write,
    /**
     * @defgroup Problem counter.
     */
    problem_count,
} problem_code_t;

/**
 * DESCRIPTION
 *
 * @brief Report a problem that occurred during the runtime of Leto. This
 * is usually a wrapper around a print statement, but if the error should
 * be fatal then the process is killed. See @file Reporter.c for the list
 * of fatal errors.
 *
 * PARAMETERS
 *
 * @param problem The problem structure, containing all the needed
 * information about the problem and its conception.
 * @param file The file in which the report originated. This is assigned by
 * CMake during the build process.
 * @param function The name of the function that reported the error. This
 * is typically retrieved via __func__.
 * @param line The line on which the error occurred. This can be easily
 * grabbed with the standard __LINE__ macro provided as per the C standard.
 *
 * RETURN VALUE
 *
 * @return If the passed problem is an error, this function will not return
 * even void, and the process will quit when it ends.
 *
 * WARNINGS
 *
 * This is the reporter interface, but there is still one possible warning
 * it can throw.
 * @warning null_param -- If either of the strings passed to the function
 * are NULL, we report this warning to protect the program from a fatal
 * error.
 *
 * ERRORS
 *
 * Two errors can potentially be thrown by this function.
 * @exception failed_buffer -- If the process has no more memory and the
 * reported problem is a warning, this error will be thrown and the process
 * will exit.
 * @exception time_error -- If on Linux and the time-grab function fails
 * for warning reporting, this error will be thrown and the process will
 * exit.
 *
 */
void LetoReport_(problem_code_t problem, const char* file,
                 const char* function, uint32_t line);

/**
 * @brief A macro to make reporting errors much easier, filling in all the
 * ridiculous macros for you.
 */
#define LetoReport(problem)                                               \
    LetoReport_(problem, FILENAME, __func__, __LINE__)

/**
 * DESCRIPTION
 *
 * @brief Get the last reported problem's code identifier. This is provided
 * to help functions potentially solve problems that happen within them or
 * their child function calls.
 *
 * PARAMETERS
 *
 * Nothing of note.
 *
 * RETURN VALUE
 *
 * @return The last problem code reported.
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
problem_code_t LetoGetWarning(void);

#endif // __LETO__REPORTER__
