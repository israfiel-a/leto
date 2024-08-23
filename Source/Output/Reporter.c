#include "Reporter.h"
#include <Diagnostic/Time.h>
#include <Utilities/Strings.h>
#include <errno.h>
#include <glad.h>
#include <glfw3.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    union
    {
        enum
        {
            os = 0x0111E,
            glfw = 0x112E,
            opengl = 0x113E,
            leto = 0x114E,
        } error;
        enum
        {
            user_error = 0x111C,
            program_error = 0x112C
        } warning;
    } type;
    const char* name;
    const char* description;
} reported_message_t;

static const reported_message_t errors[error_code_count] = {
    [failed_allocation] = {{os},
                           "failed_allocation",
                           "failed to allocate memory"},
    [time_get_error] = {{os}, "time_get_error", "failed to get time"},
    [glfw_init_failed] = {{glfw}, "glfw_init_failed", "glfw init failed"},
    [glfw_window_create_failed] = {{glfw},
                                   "glfw_window_create_failed",
                                   "glfw window creation failed"},
    [glfw_monitor_get_failed] = {{glfw},
                                 "glfw_monitor_get_failed",
                                 "glfw failed to grab primary monitor"},
    [opengl_init_failed] = {
        {opengl}, "opengl_init_failed", "failed to get opengl context"}};

static const reported_message_t warnings[warning_code_count] = {
    [null_error] = {{user_error},
                    "null_error",
                    "passed a null error to the reporter"},
    [null_warning] = {{user_error},
                      "null_warning",
                      "passed a null warning to the reporter"},
    [string_overconcat] = {{program_error},
                           "string_overconcat",
                           "string implicitly concated"},
    [preemptive_window_free] = {{user_error},
                                "preemptive_window_free",
                                "freed window before creation"},
    [double_window_creation] = {
        {user_error}, "double_window_creation", "created window twice"}};

void ReportError_(const char* file, const char* function, uint32_t line,
                  error_code_t code)
{
    if (code == error_code_count) { exit(EXIT_FAILURE); }
    const reported_message_t reported_error = errors[code];

    int extra_info = 0;
    const char* extra_info_desc = NULL;
    switch (reported_error.type.error)
    {
        case os:     extra_info = errno; break;
        case glfw:   extra_info = glfwGetError(&extra_info_desc); break;
        case opengl: extra_info = glGetError(); break;
        default:     break;
    }

    printf("\033[1;31m--\n-- !! Leto Error !!\n-- From:\033[0;31m %s :: "
           "%s(), ln. %d\n\033[1m-- Info: \n--   Kind:\033[0;31m "
           "0x%x\n\033[1m--   Code:\033[0;31m %d :: %s\n\033[1m--   "
           "Description:\033[0;31m %s\n--    Extra:\033[0;31m "
           "%d :: %s\033[1m\n--\033[0m\n",
           file, function, line, reported_error.type.error, code,
           reported_error.name, reported_error.description, extra_info,
           extra_info_desc);

    exit(EXIT_FAILURE);
}

void ReportWarning_(const char* file, const char* function, uint32_t line,
                    warning_code_t code)
{
    if (code == warning_code_count)
    {
        ReportWarning(null_warning);
        return;
    }
    const reported_message_t reported_warning = warnings[code];
    timestamp_t timestamp = TIMESTAMP_INITIALIZER;
    GetTimestamp(&timestamp, shortened);

    printf("\033[1;33m--\n-- !! Leto Warning !!\n-- At Timestamp: "
           "\033[0;33m%s\n\033[1m-- From:\033[0;33m %s :: %s(), ln. "
           "%d\n\033[1m-- Info: \n--   Kind:\033[0;33m 0x%x\n\033[1m--   "
           "Code:\033[0;33m %d :: %s\n\033[1m--   Description:\033[0;33m "
           "%s\033[1m\n--\033[0m\n",
           timestamp.string, file, function, line,
           reported_warning.type.warning, code, reported_warning.name,
           reported_warning.description);
}
