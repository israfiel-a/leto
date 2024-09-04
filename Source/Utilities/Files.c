/**
 * @file Files.c
 * @author Israfiel (https://github.com/israfiel-a)
 * @brief Provides the implementation of the public interface defined in
 * @file Files.h.
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

#include "Files.h"             // Public interface parent
#include <Output/Reporter.h>   // Error / warning reporter
#include <Utilities/Strings.h> // String utilities
#include <stdlib.h>            // Malloc, calloc, etc.
#include <string.h>            // Standard string utilities

/**
 * DESCRIPTION
 *
 * @brief Open a file. This is a simple abstraction over the @ref fopen and
 * @ref fopen_s functions, so Leto can be compliant security-wise
 * cross-platform. Nothing is allocated besides the file handle. This
 * function is only called by @ref LetoOpenFile.
 *
 * PARAMETERS
 *
 * @param path The full path to the file. This can be relative or absolute,
 * but must lead to a file or valid file location if we're writing to a
 * new file.
 * @param mode The mode to open the file in. This can be any value
 * described by @enum file_mode_t.
 *
 * RETURN VALUE
 *
 * @return The handle of the newly opened file object.
 *
 * WARNINGS
 *
 * One warning can be thrown by this function.
 * @warning null_string -- If either @param path or @param mode is passed
 * to the function as NULL, this warning is thrown and NULL is returned.
 *
 * ERRORS
 *
 * One error can be thrown by this function.
 * @exception file_open_failed -- If the file open operation fails, this
 * error is thrown.
 *
 */
static FILE* OpenFile_(const char* path, const char* mode)
{
    if (path == NULL || mode == NULL)
    {
        LetoReportWarning(null_string);
        return NULL;
    }

    FILE* opened_file = NULL;

// MSVC complains if we use fopen instead of fopen_s, so we just compile
// different code depending on the OS.
#if defined(__LETO__LINUX__)
    opened_file = fopen(path, mode);
    if (opened_file == NULL) LetoReportError(file_open_failed);
#elif defined(__LETO__WINDOWS__)
    if (fopen_s(&opened_file, path, mode) != 0)
        LetoReportError(file_open_failed);
#endif

    return opened_file;
}

/**
 * DESCRIPTION
 *
 * @brief Get the size of the file. All file positioners are reset to the
 * very beginning of the file on method completion.
 *
 * PARAMETERS
 *
 * @param file The handle of the file whose size we're trying to read.
 *
 * RETURN VALUE
 *
 * @return The size of the file as it was when the method was called.
 *
 * WARNINGS
 *
 * One warning can be thrown by this function.
 * @warning null_object -- If the file handle passed to the function is
 * NULL, this error is thrown and 0 is returned.
 *
 * ERRORS
 *
 * Two errors can be thrown by this function.
 * @exception file_positioner_set_failed -- If we fail to set the file
 * positioner, either to the end or the beginning, this error is thrown and
 * the process exits. This error can be thrown in two different places,
 * like implied earlier; either when we set the positioner to the beginning
 * or the end of the file.
 * @exception file_positioner_get_failed -- If we fail to grab the value of
 * the file positioner, this error is thrown and the process exits.
 *
 */
static size_t GetFileSize_(FILE* file)
{
    if (file == NULL)
    {
        LetoReportWarning(null_object);
        return 0;
    }

    if (fseek(file, 0L, SEEK_END) == -1)
        LetoReportError(file_positioner_set_failed);

    long size = ftell(file);
    if (size == -1) LetoReportError(file_positioner_get_failed);
    if (fseek(file, 0L, SEEK_SET) == -1)
        LetoReportError(file_positioner_set_failed);

    return (size_t)size;
}

file_t* LetoOpenFile(file_mode_t mode, const char* path)
{
    if (path == NULL)
    {
        LetoReportWarning(null_string);
        return NULL;
    }

    file_t* opened_file = malloc(sizeof(file_t));
    if (opened_file == NULL) LetoReportError(failed_allocation);
    *opened_file =
        (file_t){NULL, LetoStringMalloc(strlen(path)), 0, mode, NULL};
    strcpy((char*)opened_file->path, path);

    switch (mode)
    {
        case r:  opened_file->handle = OpenFile_(path, "rb"); break;
        case w:  opened_file->handle = OpenFile_(path, "wb"); break;
        case a:  opened_file->handle = OpenFile_(path, "ab"); break;
        case rw: opened_file->handle = OpenFile_(path, "wb+"); break;
        case ra: opened_file->handle = OpenFile_(path, "ab+"); break;
    }
    opened_file->size = GetFileSize_(opened_file->handle);

    return opened_file;
}

void LetoCloseFile(file_t* file)
{
    if (file == NULL)
    {
        LetoReportWarning(null_object);
        return;
    }

    if (file->handle == NULL)
    {
        LetoReportWarning(file_invalid);
        return;
    }

    if (fclose(file->handle) == EOF) LetoReportError(file_close_failed);
    LetoStringFree(&file->path);
    if (file->contents != NULL) free(file->contents);
    free(file);
}

void LetoReadFile(file_t* file)
{
    if (file == NULL)
    {
        LetoReportWarning(null_object);
        return;
    }

    if (file->handle == NULL)
    {
        LetoReportWarning(file_invalid);
        return;
    }

    file->contents = calloc(file->size, 1);
    if (fread(file->contents, 1, file->size, file->handle) != file->size)
        LetoReportError(file_read_failed);
    if (fseek(file->handle, 0L, SEEK_SET) == -1)
        LetoReportError(file_positioner_set_failed);
}

uint8_t* LetoReadFileP(const char* path)
{
    file_t* opened_file = LetoOpenFile(r, path);
    LetoReadFile(opened_file);

    uint8_t* buffer = malloc(opened_file->size);
    (void)memcpy(buffer, opened_file->contents, opened_file->size);

    LetoCloseFile(opened_file);
    return buffer;
}

char* LetoReadFilePC(const char* path)
{
    file_t* opened_file = LetoOpenFile(r, path);
    LetoReadFile(opened_file);

    char* buffer = LetoStringCalloc(opened_file->size);
    (void)strncpy(buffer, (char*)opened_file->contents,
                  opened_file->size - 1);

    LetoCloseFile(opened_file);
    return buffer;
}

uint8_t* LetoReadFilePV(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    char* new_path = LetoStringCreateV(MAX_PATH_LENGTH, format, args);
    va_end(args);

    file_t* opened_file = LetoOpenFile(r, new_path);
    LetoReadFile(opened_file);

    uint8_t* buffer = malloc(opened_file->size);
    (void)memcpy(buffer, opened_file->contents, opened_file->size);

    LetoCloseFile(opened_file);
    return buffer;
}

char* LetoReadFilePCV(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    char* new_path = LetoStringCreateV(MAX_PATH_LENGTH, format, args);
    va_end(args);

    file_t* opened_file = LetoOpenFile(r, new_path);
    LetoReadFile(opened_file);

    char* buffer = LetoStringCalloc(opened_file->size);
    (void)strncpy(buffer, (char*)opened_file->contents,
                  opened_file->size - 1);

    LetoCloseFile(opened_file);
    LetoStringFree(&new_path);
    return buffer;
}
