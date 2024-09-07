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

#include "Reporter.h"             // Public interface parents
#include <Diagnostic/Time.h>      // Time utilities
#include <Utilities/Attributes.h> // Cross-platform attributes
#include <Utilities/Strings.h>    // String utilities
#include <Utilities/Subshell.h>   // Subshell functionality
#include <stddef.h>               // Standard macro definitions
#include <stdio.h>                // Standard I/O functionality
#include <stdlib.h>               // exit()

#define ERROR_FORMAT "%s() in %s @ %d :: %s -- %s, type: 0x%x\n"

static problem_code_t last_warning = problem_count;

static __LETO__NORETURN__ PrintError_(const problem_t* problem,
                                      const char* file,
                                      const char* function, uint32_t line)
{
#if defined(__LETO__LINUX__)
    if (CheckLibNotify())
    {
        char* command = malloc(256);
        if (command == NULL) exit(EXIT_FAILURE);
        snprintf(command, 256,
                 "notify-send -u critical -t -1 -a Leto \"Leto Error "
                 "Report\" \"" ERROR_FORMAT "\"",
                 function, file, line, problem->name, problem->description,
                 problem->type);

        RunCommand(command);
    }
    else
        printf(ERROR_FORMAT, function, file, line, problem->name,
               problem->description, problem->type);
#elif defined(__LETO__WINDOWS__)
//! todo
#endif

    exit(EXIT_FAILURE);
}

void LetoReport_(problem_code_t problem, const char* file,
                 const char* function, uint32_t line)
{
    if (problem == problem_count) return;
    if (file == NULL || function == NULL) LetoReport(null_param);

    const problem_t reported_problem = problems[problem];
    if (!reported_problem.fatal)
    {
        timestamp_t current_time;
        LetoGetTimestamp(&current_time, bracketed);

        printf("%s " ERROR_FORMAT, current_time.string, function, file,
               line, reported_problem.name, reported_problem.description,
               reported_problem.type);
        last_warning = problem;
    }
    else PrintError_(&reported_problem, file, function, line);
}

problem_code_t LetoGetWarning(void) { return last_warning; }
