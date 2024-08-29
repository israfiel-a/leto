/**
 * @file Strings.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides utility functions for sanitizing string operation, like
 * allocating proper buffers and reporting overconcatenation and the like.
 * @date 2024-08-22
 *
 * @copyright (c) 2024 - the Leto Team
 */

#ifndef __LETO__STRINGS__
#define __LETO__STRINGS__

#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

char* StringMalloc(size_t string_length);
char* StringCalloc(size_t string_length);
void StringFree(char** string);

void SetStringF(bool warn_overcat, char** buffer, size_t max_buffer_length,
                const char* format, ...);
char* StringCreate(size_t max_buffer_size, const char* format, ...);
char* StringCreateV(size_t max_buffer_size, const char* format,
                    va_list args);

#endif // __LETO__STRINGS__
