/**
 * @file Macros.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides some utility macros that are in use across the
 * application, like a stringify macro.
 * @date 2024-08-22
 *
 * @copyright (c) 2024 - Israfiel
 */

#ifndef __LETO__MACROS__
#define __LETO__MACROS__

#define __STR_INTERNAL(x) #x
#define __STRINGIFY(x) __STR_INTERNAL(x)

/**
 * @brief A simple macro to convert nanoseconds to milliseconds. This is
 * literally just a division by 1000000.
 */
#define NSEC_TO_MSEC(value) (uint64_t) value / 1000000

#endif // __LETO__MACROS__
