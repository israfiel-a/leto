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

#include <stdbool.h>
#include <stdint.h>

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
    time_error,
    /**
     * @defgroup Specific interface problems.
     */
    glfw_init,
    glfw_monitor,
    gl_init,
    gl_shader_comp,
    gl_shader_bad,
    null_window,
    window_null,
    file_null,
    file_pos_get,
    file_pos_set,
    file_read,
    file_write,
    /**
     * @defgroup Problem counter.
     */
    problem_count,
} problem_code_t;

typedef struct
{
    const char* name;
    const char* description;
    bool fatal;
    enum
    {
        /**
         * @brief This flag represents that the problem was out of our
         * control, like an allocation failure or failed file operation.
         */
        os = 0x111E,
        glfw = 0x112E,
        opengl = 0x113E,
        leto = 0x114E
    } type;
} problem_t;

static const problem_t problems[problem_count] = {
    {"null_param", "a null parameter was passed", false, leto},
    {"failed_buffer", "an allocation failure occurred", true, os},
    {"small_buffer", "tried to access past buffer bounds", false, leto},
    {"no_such_value", "no equal value found in list", false, leto},
    {"array_full", "tried to assign past array bounds", false, leto},
    {"time_error", "failed to get time", true, os},
    {"glfw_init", "failed to initialize glfw", true, glfw},
    {"glfw_monitor", "failed to get glfw monitor", true, glfw},
    {"gl_init", "failed to initialize glad/opengl", true, opengl},
    {"gl_shader_comp", "failed to compile shader", true, opengl},
    {"gl_shader_bad", "failed to utilize shader", true, opengl},
    {"window_null", "call made to nonexistent window", false, leto},
    {"null_window", "failed to create window", true, glfw},
    {"file_null", "call made to nonexistent file", false, leto},
    {"file_pos_get", "failed to get file positioner", false, os},
    {"file_pos_set", "failed to set file positioner", false, os},
    {"file_read", "failed to read (from?) file", false, os},
    {"file_write", "failed to write to file", false, os},
};

void LetoReport_(problem_code_t problem, const char* file,
                 const char* function, uint32_t line);

#define LetoReport(problem)                                               \
    LetoReport_(problem, FILENAME, __func__, __LINE__)

problem_code_t LetoGetWarning(void);

#endif // __LETO__REPORTER__
