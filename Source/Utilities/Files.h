/**
 * @file Files.h
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides the interface for opening, closing, writing, and reading
 * to files safely.
 * @date 2024-09-02
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

#ifndef __LETO__FILES__
#define __LETO__FILES__

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef enum
{
    r,
    w,
    a,
    rw,
    ra,
} file_mode_t;

typedef struct
{
    FILE* handle;
    char* path;
    size_t size;
    file_mode_t permissions;
    uint8_t* contents;
} file_t;

file_t* LetoOpenFile(file_mode_t mode, const char* path);

void LetoCloseFile(file_t* file);

void LetoReadFile(file_t* file);

uint8_t* LetoReadFileP(const char* path);

char* LetoReadFilePC(const char* path);

uint8_t* LetoReadFilePV(const char* format, ...);

char* LetoReadFilePCV(const char* format, ...);

#endif // __LETO__FILES__
