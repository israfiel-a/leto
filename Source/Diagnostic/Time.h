/**
 * @file Time.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides functionality for time polling, both numerical and
 * string.
 * @date 2024-08-22
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef __LETO__TIME__
#define __LETO__TIME__

#include <stdint.h>

#define TIMESTAMP_STRING_MAX_LENGTH 128

typedef enum
{
    full,
    shortened,
    bracketed
} timestamp_format_t;

typedef struct
{
    char* string;
    timestamp_format_t format;
    uint32_t minutes;
    uint32_t milliseconds;
    uint32_t seconds;
} timestamp_t;

#define TIMESTAMP_INITIALIZER                                             \
    (timestamp_t) { 0, 0, 0, NULL, full }

void GetTimeRaw(uint64_t* ms);
void GetTimestamp(timestamp_t* storage, timestamp_format_t format);

#endif // __LETO__TIME__
