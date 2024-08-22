/**
 * @file Error.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides functionality for reporting errors to the end user,
 * either in the command line or via an operating-system-specific
 * notification software.
 * @date 2024-08-22
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef __LETO__REPORTER__
#define __LETO__REPORTER__

#include <stdint.h>

typedef enum
{
    failed_allocation,
    time_get_error,
    glfw_init_failed,
    opengl_init_failed,
    error_code_count
} error_code_t;

/**
 * @brief
 *
 * @param file
 * @param function
 * @param line
 * @param code
 * @return _Noreturn
 * @todo implement notification functionality
 */
_Noreturn void ReportError_(const char* file, const char* function,
                            uint32_t line, error_code_t code);

#define ReportError(code) ReportError_(FILENAME, __func__, __LINE__, code)

typedef enum
{
    null_error,
    null_warning,
    string_overconcat,
    warning_code_count
} warning_code_t;

void ReportWarning_(const char* file, const char* function, uint32_t line,
                    warning_code_t code);

#define ReportWarning(code)                                               \
    ReportWarning_(FILENAME, __func__, __LINE__, code)

#endif // __LETO__REPORTER__
