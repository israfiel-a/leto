/**
 * @file Strings.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides utility functions for sanitizing string operation, like
 * allocating proper buffers and reporting overconcatenation and the like.
 * @date 2024-08-22
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef __LETO__STRINGS__
#define __LETO__STRINGS__

#include <stdbool.h>
#include <string.h>

char* StringMalloc(size_t string_length);

void FormattedSetString(bool warn_overcat, char** buffer,
                        size_t max_buffer_length, const char* format, ...);

#endif // __LETO__STRINGS__
