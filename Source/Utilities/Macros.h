/**
 * @file Macros.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides some utility macros that are in use across the
 * application, like a stringify macro.
 * @date 2024-08-22
 *
 * @copyright (c) 2024 - the Leto Team
 */

#ifndef __LETO__MACROS__
#define __LETO__MACROS__

#define __STR_INTERNAL(x) #x
#define __STRINGIFY(x) __STR_INTERNAL(x)

#define BYTES_TO_MEGABYTES(value) (value) / 1000000
#define KILOBYTES_TO_MEGABYTES(value) (value) / 1000

#define MAX_PATH_LENGTH 256

/**
 * @brief A simple macro to convert nanoseconds to milliseconds. This is
 * literally just a division by 1000000.
 */
#define NSEC_TO_MSEC(value) ((uint64_t)(value)) / 1000000

#define SYSTIME_TO_MS(time_struct)                                        \
    ((LONGLONG)time_struct.dwLowDateTime +                                \
     ((LONGLONG)(time_struct.dwHighDateTime) << 32LL)) /                  \
        10000

#endif // __LETO__MACROS__
