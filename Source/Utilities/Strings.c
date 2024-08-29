#include "Strings.h"
#include <Output/Reporter.h>
#include <stdio.h>
#include <stdlib.h>

char* StringMalloc(size_t string_length)
{
    void* allocated = malloc(string_length + 1);
    if (allocated == NULL) ReportError(failed_allocation);
    return allocated;
}

char* StringCalloc(size_t string_length)
{
    void* allocated = calloc(string_length + 1, 1);
    if (allocated == NULL) ReportError(failed_allocation);
    return allocated;
}

void StringFree(char** string)
{
    free(*string);
    *string = NULL;
}

void SetStringF(bool warn_overcat, char** buffer, size_t max_string_length,
                const char* format, ...)
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

    free(temp_buffer);
    va_end(args);
}

char* StringCreate(size_t max_buffer_size, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    char* buffer = StringMalloc(max_buffer_size);
    if (vsnprintf(buffer, max_buffer_size, format, args) > max_buffer_size)
        ReportWarning(string_overconcat);
    va_end(args);

    buffer = realloc(buffer, strlen(buffer) + 1);
    if (buffer == NULL) ReportError(failed_allocation);
    return buffer;
}

char* StringCreateV(size_t max_buffer_size, const char* format,
                    va_list args)
{
    if (format == NULL)
    {
        ReportWarning(null_string);
        return NULL;
    }

    if (args == NULL)
    {
        ReportWarning(null_object);
        return NULL;
    }

    char* buffer = StringMalloc(max_buffer_size);
    if (vsnprintf(buffer, max_buffer_size, format, args) > max_buffer_size)
        ReportWarning(string_overconcat);

    buffer = realloc(buffer, strlen(buffer) + 1);
    if (buffer == NULL) ReportError(failed_allocation);
    return buffer;
}
