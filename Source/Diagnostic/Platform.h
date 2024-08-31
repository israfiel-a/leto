/**
 * @file Platform.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides macros to determine the platform, build type, version,
 * etc. of the current Leto binaries.
 * @date 2024-08-22
 *
 * @copyright (c) 2024 - the Leto Team
 * This document is under the GNU Affero General Public License v3.0. It
 * can be modified and distributed (commercially or otherwise) freely, and
 * can be used privately and within patents. No liability or warranty is
 * guaranteed. However, on use, the user must state license and copyright,
 * any changes made, and disclose the source of the document. For more
 * information see the @file LICENSE.md file included with this
 * distribution of the Leto source code.
 */

#ifndef __LETO__PLATFORM__
#define __LETO__PLATFORM__

// Utility macros.
#include <Utilities/Macros.h>
// Fixed-width integer types as described by the C11 standard.
#include <stdint.h>

// Figure out the architecture of the host machine. Leto will only run on
// x86_64 processors (at least, to my knowledge) and as such we throw an
// error for anything else.
#if defined(__x86_64__) || defined(_M_X64)
    #define __LETO__X86_64__
    #define __LETO__ARCH__ "x86_64"
#elif defined(i386) || defined(__i386__) || defined(__i386) ||            \
    defined(_M_IX86)
    #define __LETO__X86_32__
    #define __LETO__ARCH__ "x86_32"
#else
    #error "Incompatible machine architecture."
#endif

// Figure out the platform Leto is running on. We only poll for
// Windows or Linux, and immediately throw an error for anything
// else.
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) ||            \
    defined(__MINGW32__) || BUILD_PLATFORM == 0
    #define __LETO__WINDOWS__
    #define __LETO__PLATFORM__TYPE__ "windows"
#elif defined(linux) || BUILD_PLATFORM == 1
    #define __LETO__LINUX__
    #define __LETO__PLATFORM__TYPE__ "linux"
#else
    #error "Incompatible operating system."
#endif

// If we're on Linux, figure out which display server we're using.
// This is here instead of above, in the OS block, to prevent
// needless nesting.
#if defined(__LETO__LINUX__) && DISPLAY_SERVER == 0
    #define __LETO__WAYLAND__
    #define __LETO__DISPLAY_SERVER__ "wayland"
#elif defined(__LETO__LINUX__) && DISPLAY_SERVER == 1
    #define __LETO__X11__
    #define __LETO__DISPLAY_SERVER__ "x11"
#endif

// Stringify the build type and turn it into a boolean value as
// well. This is for ease of use within the program.
#if BUILD_TYPE == 0
    #define __LETO__DEBUG__
    #define __LETO__BUILD__TYPE__ "debug"
#elif BUILD_TYPE == 1
    #define __LETO__RELEASE__
    #define __LETO__BUILD__TYPE__ "release"
#else
    #error "Unrecognized build type."
#endif

/**
 * @brief This is the major version of Leto. This value only really
 * increases for major updates, like system overhauls and the like.
 */
#define __LETO__VERSION__MAJOR__ 1

/**
 * @brief This is the minor version of Leto. This changes with
 * every change that will affect a normal user, such as major UI changes.
 * This value resets to 0 every new major version.
 */
#define __LETO__VERSION__MINOR__ 3

/**
 * @brief This is the tweak version of Leto. This changes with
 * every change that will only affect the application's inner
 * workings, like a slight optimization. This value will reset to 0
 * every new minor version.
 */
#define __LETO__VERSION__TWEAK__ 7

/**
 * @brief This is the string represenation of Leto's base version,
 * without platform or build type. For one @b without said
 * omissions, see @ref
 * __LETO__VERSION__STRING__FULL__.
 */
#define __LETO__VERSION__STRING__                                         \
    __STRINGIFY(__LETO__VERSION__MAJOR__)                                 \
    "." __STRINGIFY(__LETO__VERSION__MINOR__) "." __STRINGIFY(            \
        __LETO__VERSION__TWEAK__)

/**
 * @brief This is the full string representation of Leto's version.
 * Its format is [MAJOR].[MINOR].[TWEAK]-[PLATFORM]-[ARCH]-[BUILD_TYPE].
 * For a string without the build and platform, see @ref
 * __LETO__VERSION__STRING__.
 */
#define __LETO__VERSION__STRING__FULL__                                   \
    __STRINGIFY(__LETO__VERSION__MAJOR__)                                 \
    "." __STRINGIFY(__LETO__VERSION__MINOR__) "." __STRINGIFY(            \
        __LETO__VERSION__TWEAK__) "-" __LETO__PLATFORM__TYPE__            \
                                  "-" __LETO__ARCH__                      \
                                  "."__LETO__BUILD__TYPE__

#endif // __LETO__PLATFORM__
