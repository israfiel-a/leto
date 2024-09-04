#include "Reporter.h"
#include <Diagnostic/Time.h>
#include <errno.h>
#include <gl.h>
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
    [file_open_failed] = {{os}, "file_open_failed", "failed to open file"},
    [file_close_failed] = {{os},
                           "file_close_failed",
                           "failed to close file"},
    [file_positioner_set_failed] = {{os},
                                    "file_positioner_set_failed",
                                    "failed to set file position marker"},
    [file_positioner_get_failed] = {{os},
                                    "file_positioner_get_failed",
                                    "failed to get file position marker"},
    [file_read_failed] = {{os}, "file_read_failed", "failed to read file"},
    [glfw_init_failed] = {{glfw}, "glfw_init_failed", "glfw init failed"},
    [glfw_window_create_failed] = {{glfw},
                                   "glfw_window_create_failed",
                                   "glfw window creation failed"},
    [glfw_monitor_get_failed] = {{glfw},
                                 "glfw_monitor_get_failed",
                                 "glfw failed to grab primary monitor"},
    [opengl_init_failed] = {{opengl},
                            "opengl_init_failed",
                            "failed to get opengl context"},
    [opengl_shader_compilation_failed] =
        {{opengl},
         "opengl_shader_compilation_failed",
         "failed to compile shader (see prior printout for info)"},
    [opengl_malformed_shader] = {{opengl},
                                 "opengl_malformed_shader",
                                 "an invalid shader object was used"}};

static const reported_message_t warnings[warning_code_count] = {
    [null_error] = {{user_error},
                    "null_error",
                    "passed a null error to the reporter"},
    [null_warning] = {{user_error},
                      "null_warning",
                      "passed a null warning to the reporter"},
    [null_object] = {{user_error}, "passed a null object to a function"},
    [null_string] = {{user_error},
                     "null_string",
                     "passed a null string to a function"},
    [string_overconcat] = {{program_error},
                           "string_overconcat",
                           "string implicitly concated"},
    [preemptive_window_free] = {{user_error},
                                "preemptive_window_free",
                                "freed window before creation"},
    [double_window_creation] = {{user_error},
                                "double_window_creation",
                                "created window twice"},
    [preemptive_buffer_swap] = {{user_error},
                                "preemptive_buffer_swap",
                                "swapped buffers without window created"},
    [preemptive_window_info] =
        {{user_error},
         "preemptive_window_info",
         "tried to get window info without creating window"},
    [preemptive_file_close] =
        {{user_error},
         "preemptive_file_close",
         "tried to close a file before it was opened"},
    [no_such_shader] = {{program_error},
                        "no_such_shader",
                        "no such shader found"},
    [shader_list_full] = {{program_error},
                          "shader_list_full",
                          "the opengl shader list is full to capacity"},
    [file_path_invalid] = {{user_error},
                           "file_path_invalid",
                           "invalid path was passed to file function"},
    [file_invalid] = {{program_error},
                      "file_invalid",
                      "a file handler has become invalid"}};

void LetoReportError_(const char* file, const char* function,
                      uint32_t line, error_code_t code)
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
        case leto:
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

void LetoReportWarning_(const char* file, const char* function,
                        uint32_t line, warning_code_t code)
{
    if (code == warning_code_count)
    {
        LetoReportWarning(null_warning);
        return;
    }
    const reported_message_t reported_warning = warnings[code];
    timestamp_t timestamp = TIMESTAMP_INITIALIZER;
    LetoGetTimestamp(&timestamp, shortened);

    printf("\033[1;33m--\n-- !! Leto Warning !!\n-- At Timestamp: "
           "\033[0;33m%s\n\033[1m-- From:\033[0;33m %s :: %s(), ln. "
           "%d\n\033[1m-- Info: \n--   Kind:\033[0;33m 0x%x\n\033[1m--   "
           "Code:\033[0;33m %d :: %s\n\033[1m--   Description:\033[0;33m "
           "%s\033[1m\n--\033[0m\n",
           timestamp.string, file, function, line,
           reported_warning.type.warning, code, reported_warning.name,
           reported_warning.description);
}
