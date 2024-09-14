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
#include <stddef.h>

char* LetoStringMalloc(size_t string_length);
char* LetoStringCalloc(size_t string_length);
void LetoStringFree(char** string);

void LetoSetStringF(bool warn_overcat, char** buffer,
                    size_t max_buffer_length, const char* format, ...);
char* LetoStringCreate(size_t max_buffer_size, const char* format, ...);
char* LetoStringCreateV(size_t max_buffer_size, const char* format,
                        va_list args);

char** LetoStringSplit(char* string, char delimiter);

#endif // __LETO__STRINGS__
