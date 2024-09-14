#include "strings.h"
#include <io/reporter.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* LetoStringMalloc(size_t string_length)
{
    void* allocated = malloc(string_length + 1);
    if (allocated == NULL) LetoReport(failed_buffer);
    return allocated;
}

char* LetoStringCalloc(size_t string_length)
{
    void* allocated = calloc(string_length + 1, 1);
    if (allocated == NULL) LetoReport(failed_buffer);
    return allocated;
}

void LetoStringFree(char** string)
{
    if (*string == NULL) return;
    free(*string);
    *string = NULL;
}

void LetoSetStringF(bool warn_overcat, char** buffer,
                    size_t max_string_length, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    char* temp_buffer = LetoStringMalloc(max_string_length);
    int attempted_characters =
        vsnprintf(temp_buffer, max_string_length, format, args);

    if ((attempted_characters < 0 ||
         (size_t)attempted_characters > max_string_length) &&
        warn_overcat)
        LetoReport(small_buffer);

    if (*buffer != NULL) free(*buffer);
    *buffer = LetoStringMalloc(strlen(temp_buffer));
    strcpy(*buffer, temp_buffer);

    free(temp_buffer);
    va_end(args);
}

char* LetoStringCreate(size_t max_buffer_size, const char* format, ...)
{
    va_list args;
    va_start(args, format);

    char* buffer = LetoStringMalloc(max_buffer_size);
    int attempted_characters =
        vsnprintf(buffer, max_buffer_size, format, args);

    if (attempted_characters < 0 ||
        (size_t)attempted_characters > max_buffer_size)
        LetoReport(small_buffer);
    va_end(args);

    // This whole block is to satisfy MSVC.
    char* temp_buffer = realloc(buffer, strlen(buffer) + 1);
    if (temp_buffer == NULL) LetoReport(failed_buffer);
    buffer = temp_buffer;

    return buffer;
}

char* LetoStringCreateV(size_t max_buffer_size, const char* format,
                        va_list args)
{
    if (format == NULL || args == NULL)
    {
        LetoReport(null_param);
        return NULL;
    }

    char* buffer = LetoStringMalloc(max_buffer_size);
    int attempted_characters =
        vsnprintf(buffer, max_buffer_size, format, args);

    if (attempted_characters < 0 ||
        (size_t)attempted_characters > max_buffer_size)
        LetoReport(small_buffer);

    // This whole block is to satisfy MSVC.
    char* temp_buffer = realloc(buffer, strlen(buffer) + 1);
    if (temp_buffer == NULL) LetoReport(failed_buffer);
    buffer = temp_buffer;
    return buffer;
}

char** LetoStringSplit(char* string, char delimiter)
{
    char** split_strings = NULL;

    char *temporary_copy = strdup(string),
         *temporary_copy_beginning = temporary_copy;
    char* last_delimiter = NULL;
    size_t delimiter_count = 0;

    while (*temporary_copy != '\0')
    {
        if (delimiter == *temporary_copy)
        {
            delimiter_count++;
            last_delimiter = temporary_copy;
        }
        temporary_copy++;
    }
    // add space for trailing delim
    delimiter_count += last_delimiter < (string + strlen(string) - 1);
    // null string terminator
    delimiter_count++;

    split_strings = malloc(sizeof(char*) * delimiter_count);
    if (split_strings == NULL) LetoReport(failed_buffer);

    size_t current_index = 0;
    char terminated_delimiter[2] = {delimiter, 0};
    char* token = strsep(&temporary_copy, terminated_delimiter);
    while (token != NULL)
    {
        split_strings[current_index] = token;
        current_index += 1;
        token = strsep(&temporary_copy, terminated_delimiter);
    }
    printf("%zu :: %zu\n", delimiter_count, current_index);
    split_strings[delimiter_count] = NULL;

    free(temporary_copy_beginning);

    return split_strings;
}
