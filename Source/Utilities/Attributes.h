/**
 * @file Attributes.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides platform-independent attribute macros for functions and
 * variables. These should @b always be used in place of platform-specific
 * versions.
 * @date 2024-08-31
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

#ifndef __LETO__ATTRIBUTES__
#define __LETO__ATTRIBUTES__

// Platform-decision macros.
#include <Diagnostic/Platform.h>

#if defined(__LETO__LINUX__)
    #define __LETO__NORETURN__ _Noreturn void
#elif defined(__LETO__WINDOWS__)
    #define __LETO__NORETURN__ __declspec(noreturn) void
#endif

#endif // __LETO__ATTRIBUTES__
