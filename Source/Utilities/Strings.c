#include "Strings.h"
#include <Output/Reporter.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

char* StringMalloc(size_t string_length)
{
    void* allocated = malloc(string_length + 1);
    if (allocated == NULL) ReportError(failed_allocation);
    return allocated;
}

void FormattedSetString(bool warn_overcat, char** buffer,
                        size_t max_string_length, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    char* temp_buffer = StringMalloc(max_string_length);
    if (vsnprintf(temp_buffer, max_string_length, format, args) >
            max_string_length &&
        warn_overcat)
        ReportWarning(string_overconcat);

    if (*buffer != NULL) free(*buffer);
    *buffer = StringMalloc(strlen(temp_buffer));
    strcpy(*buffer, temp_buffer);

    va_end(args);
}
