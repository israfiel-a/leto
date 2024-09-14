/**
 * @file Reporter.c
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides the definition for the warning/error reporter interface
 * as provided by @file Reporter.h.
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

#include "reporter.h"             // Public interface parents
#include <diagnostic/time.h>      // Time utilities
#include <stddef.h>               // Standard macro definitions
#include <stdio.h>                // Standard I/O functionality
#include <stdlib.h>               // exit()
#include <utilities/attributes.h> // Cross-platform attributes
#include <utilities/strings.h>    // String utilities
#include <utilities/subshell.h>   // Subshell functionality

/**
 * @brief The format string for the error message. We use this multiple
 * times throughout the reporting functions to construct reports.
 */
#define ERROR_MESSAGE_FORMAT "%s() in %s @ %d :: %s -- %s, type: 0x%x\n"

/**
 * @brief This is the last warning that was sent to the reporting
 * interface. This is stored purely for getting purposes, none of the
 * reporting functions depend upon this value.
 */
static problem_code_t last_warning = problem_count;

/**
 * @brief A structure describing a problem in its entirety. This is
 * constructed internally within the @ref LetoReport_ function.
 */
typedef struct
{
    /**
     * @brief The string, human-readable name of the error. This is the
     * same as the corresponding value within @enum problem_code_t.
     */
    const char* name;
    /**
     * @brief A short and sweet description of the error. This value should
     * be sub-50 characters.
     */
    const char* description;
    /**
     * @brief This flag describes whether or not the error was fatal.
     */
    bool fatal;
    /**
     * @brief An enumerator describing what kind of problem was reported.
     */
    enum
    {
        /**
         * @brief The problem was out of our control, like an allocation
         * failure or failed file operation, not necessarily an "operating
         * system problem." Diagnosing something like that from within Leto
         * would be a bit pretentious.
         */
        os = 0x111E,
        /**
         * @brief The problem occurred in the GLFW library binaries.
         */
        glfw = 0x112E,
        /**
         * @brief The problem occurred in the OpenGL library binaries.
         */
        opengl = 0x113E,
        /**
         * @brief The problem was a result of Leto's own code. These should
         * almost never be reported in production.
         */
        leto = 0x114E
    } type;
} problem_t;

/**
 * @brief The array of problem values. This changes with each new problem
 * added to the catalogue.
 */
static const problem_t problems[problem_count] = {
    {"null_param", "a null parameter was passed", false, leto},
    {"failed_buffer", "an allocation failure occurred", true, os},
    {"small_buffer", "tried to access past buffer bounds", false, leto},
    {"no_such_value", "no equal value found in list", false, leto},
    {"array_full", "tried to assign past array bounds", false, leto},
    {"string_error", "string manipulation error", false, leto},
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

/**
 * DESCRIPTION
 *
 * @brief Print and error message and exit from the process. This can
 * either post a notification or print to stdout, depending on the
 * environment we're being run in.
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
 * @return This function does not return, ever. The calling of this
 * function always ends the process.
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
static __LETO__NORETURN__ PrintError_(const problem_t* problem,
                                      const char* file,
                                      const char* function, uint32_t line)
{
#if defined(__LETO__LINUX__)
    if (CheckLibNotify())
    {
        char* command = malloc(256);
        if (command == NULL) exit(EXIT_FAILURE); // no more mem
        snprintf(command, 256,
                 "notify-send -u critical -t -1 -a Leto \"Leto Error "
                 "Report\" \"" ERROR_MESSAGE_FORMAT "\"",
                 function, file, line, problem->name, problem->description,
                 problem->type);

        // system() wrapper
        RunCommand(command);
    }
    else
        printf(ERROR_MESSAGE_FORMAT, function, file, line, problem->name,
               problem->description, problem->type);
#elif defined(__LETO__WINDOWS__)
    (void)problem;
    (void)file;
    (void)function;
    (void)line;
//! todo
#endif

    exit(EXIT_FAILURE);
}

void LetoReport_(problem_code_t problem, const char* file,
                 const char* function, uint32_t line)
{
    if (problem == problem_count) return;
    if (file == NULL || function == NULL)
    {
        LetoReport(null_param);
        return;
    }

    const problem_t reported_problem = problems[problem];
    if (!reported_problem.fatal)
    {
        timestamp_t current_time;
        LetoGetTimestamp(&current_time, bracketed);

        printf("%s " ERROR_MESSAGE_FORMAT, current_time.string, function,
               file, line, reported_problem.name,
               reported_problem.description, reported_problem.type);
        last_warning = problem;
    }
    else PrintError_(&reported_problem, file, function, line);
}

problem_code_t LetoGetWarning(void) { return last_warning; }
