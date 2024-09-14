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

// Fixed-width integers as described by the C standard.
#include <stdint.h>
// The boolean type as described by the C standard.
#include <stdbool.h>
// IO functionality defined by the C standard.
#include <stdio.h>

/**
 * @brief An enumerator describing the various states a file can be opened
 * in; read, write, append, etc.
 */
typedef enum
{
    /**
     * @brief The file is going to be opened in read-only mode. (rb)
     */
    r,
    /**
     * @brief The file is going to be opened in write-only mode (previous
     * contents are erased). Nonexistent files will be created. (wb)
     */
    w,
    /**
     * @brief Open the file in append-only mode (content is @b not
     * overwritten). Nonexistent files will be created. (ab)
     */
    a,
    /**
     * @brief Open the file in read-write mode, meaning the file can be
     * read and any data written to it will overwrite previous contents.
     * Nonexistent files will be created. (wb+)
     */
    rw,
    /**
     * @brief Open the file in read-append mode. The file can be read from
     * and any data written to it will be added to the end of the file.
     * Nonexistent files will be created. (ab+)
     */
    ra,
} file_mode_t;

/**
 * @brief A structure describing an opened file. This structure contains
 * all information necessary for managing an opened file.
 */
typedef struct
{
    /**
     * @brief The actual file handle of the opened file. This is used for
     * file manipulations. @note Any manipulation of this pointer will
     * likely result in unintended side effects when reading or writing.
     */
    FILE* handle;
    /**
     * @brief The string representation of the file's path. This doesn't
     * ever change beyond initial file creation.
     */
    const char* path;
    /**
     * @brief The size of the file. This can be refreshed with a call to
     * @ref LetoGetFilesize, although this is rarely needed.
     */
    size_t size;
    /**
     * @brief The permissions allowed to the file. See @enum file_mode_t.
     */
    file_mode_t permissions;
    /**
     * @brief An array of each byte within the file. This is NULL unless
     * @ref LetoReadFile is called on the file.
     */
    uint8_t* contents;
} file_t;

/**
 * DESCRIPTION
 *
 * @brief Open a file that either exists within the file system, or should
 * exist within the file system (should you open the file in non-readonly
 * mode).
 *
 * PARAMETERS
 *
 * @param mode The mode you request to open the file in. See @enum
 * file_mode_t.
 * @param path The path to the file, relative or absolute.
 *
 * RETURN VALUE
 *
 * @return The newly created file object.
 *
 * WARNINGS
 *
 * Only one warning can be thrown by this function.
 * @warning null_string -- If the string @param path is NULL, this warning
 * is thrown and the method exits, returning NULL.
 *
 * ERRORS
 *
 * Four errors can be thrown by this function.
 * @exception failed_allocation -- If we fail to allocate enough space for
 * the new file structure or its path, this error will be thrown and the
 * process will exit.
 * @exception file_open_failed -- If we fail to open the file, this error
 * is thrown and the process exits.
 * @exception file_positioner_set_failed -- If we fail to set the file
 * position indicator either to the end or the beginning while retrieving
 * the file's size, this error will be thrown and the process will exit.
 * @exception file_positioner_get_failed -- If we fail to grab the file
 * position indicator, this error will be thrown and the process will exit.
 *
 */
file_t* LetoOpenFile(file_mode_t mode, const char* path);

/**
 * DESCRIPTION
 *
 * @brief Close a previously opened file object, and free all its contents.
 *
 * PARAMETERS
 *
 * @param file The file to close.
 *
 * RETURN VALUE
 *
 * Nothing of note.
 *
 * WARNINGS
 *
 * Two warnings can be thrown by this method.
 * @warning null_object -- If the file passed to this function is NULL, we
 * throw this warnings and return without doing anything.
 * @warning file_invalid -- If the inner handle of the file object is NULL,
 * this warning is thrown and the method returns.
 *
 * ERRORS
 *
 * One error can be thrown by this function.
 * @exception file_close_failed -- If, for whatever reason, we fail to @b
 * close the file, this error is thrown and the process exits.
 *
 */
void LetoCloseFile(file_t* file);

/**
 * DESCRIPTION
 *
 * @brief Update and retrieve the size of the given file object.
 *
 * PARAMETERS
 *
 * @param file The file from which to retrieve the size and the object in
 * which that size will be recorded.
 *
 * RETURN VALUE
 *
 * @return The newly-polled size of the file.
 *
 * WARNINGS
 *
 * Two warnings can be thrown by this function.
 * @warning null_object -- If the file object passed to the function is
 * NULL, this warning is thrown and the method returns 0 without doing
 * anything.
 * @warning file_invalid -- If the inner file handle of the file is NULL,
 * this warning is thrown and the method returns 0 immediately.
 *
 * ERRORS
 *
 * Two errors can be thrown by this function.
 * @exception file_positioner_set_failed -- If we fail to set the file
 * position indicator to either the front or end of the file while polling
 * for its size, this error will be thrown and the process will exit.
 * @exception file_positioner_get_failed -- If we fail to get the current
 * position of the file indicator, this error is thrown and the process
 * exits.
 *
 */
size_t LetoGetFilesize(file_t* file);

/**
 * DESCRIPTION
 *
 * @brief Read every byte from the given file, start to finish. This array
 * of bytes goes within the file object's "contents" member.
 *
 * PARAMETERS
 *
 * @param file The previously opened file object to read from.
 *
 * RETURN VALUE
 *
 * Nothing of note.
 *
 * WARNINGS
 *
 * Two warnings can be thrown by this function.
 * @warning null_object -- If the file object passed to this function is
 * NULL, this warning will be thrown and nothing will happen.
 * @warning file_invalid -- If the handle within the file object is NULL,
 * we assume the object's been corrupted and thrown this warning. The
 * function will exit immediately.
 *
 * ERRORS
 *
 * Three errors can be thrown by this function.
 * @exception failed_allocation -- If the program runs out of memory and we
 * cannot allocate space for the array of bytes representing the file, this
 * error will be thrown and the process will end.
 * @exception file_positioner_set_failed -- If we fail to set the file
 * position marker, this error is thrown and the process exits.
 * @exception file_read_failed -- If the actual reading of the file has
 * failed, we assume something's wrong with our program's I/O and exit the
 * processs.
 *
 */
void LetoReadFile(file_t* file);

/**
 * DESCRIPTION
 *
 * @brief Read the contents of the file specified. The returned buffer is
 * dynamically allocated, you must free it before it goes out of scope.
 *
 * PARAMETERS
 *
 * @param terminate A flag whether or not we should NULL-terminate the
 * returned array of bytes. This is useful for reading strings.
 * @param path The path to the file, be it absolute or relative.
 *
 * RETURN VALUE
 *
 * @return An array of bytes corresponding to each byte within the file.
 *
 * WARNINGS
 *
 * Nothing of note.
 * For warnings this function may not handle, see @ref LetoOpenFile and
 * @ref LetoReadFile.
 *
 * ERRORS
 *
 * One error can be thrown by this function.
 * @exception failed_allocation -- If the program runs out of memory and we
 * cannot allocate space for the returned array, this error will be thrown
 * and the process will end.
 * For any errors this function doesn't cover, see @ref LetoOpenFile and
 * @ref LetoReadFile.
 *
 */
uint8_t* LetoReadFileP(bool terminate, const char* path);

/**
 * DESCRIPTION
 *
 * @brief Read the contents of the file specified in the below format
 * string and subsequent concantenated arguments. The returned buffer is
 * dynamically allocated, you must free it before it goes out of scope.
 *
 * PARAMETERS
 *
 * @param terminate A flag whether or not we should NULL-terminate the
 * returned array of bytes. This is useful for reading strings.
 * @param format The format string of the function. All variadic arguments
 * will be shoved into this string.
 *
 * RETURN VALUE
 *
 * @return An array of bytes corresponding to each byte within the file.
 *
 * WARNINGS
 *
 * Nothing of note.
 * For warnings this function may not handle, see @ref LetoOpenFile,
 * @ref LetoReadFile, and @ref LetoStringCreateV.
 *
 * ERRORS
 *
 * One error can be thrown by this function.
 * @exception failed_allocation -- If the program runs out of memory and we
 * cannot allocate space for the returned array, this error will be thrown
 * and the process will end.
 * For any errors this function doesn't cover, see @ref LetoStringCreateV,
 * @ref LetoOpenFile, and @ref LetoReadFile.
 *
 */
uint8_t* LetoReadFilePV(bool terminate, const char* format, ...);

/**
 * DESCRIPTION
 *
 * @brief Write contents to a file. This operation is done using @ref
 * fwrite. If you're attempting to write a string to the file, do NOT write
 * the null terminator character (strlen(@param buffer) will do), doing so
 * will cause most text editors to refuse to handle the file. If the file
 * you're attempting to write to only allows for reading, the function
 * simply returns without doing anything.
 *
 * PARAMETERS
 *
 * @param file The previously opened file object.
 * @param buffer An array of 8-bit integers that we are going to write to
 * the file.
 * @param buffer_size The size, in bytes, of the array of 8-bit integers
 * specified by @ref buffer.
 *
 * RETURN VALUE
 *
 * Nothing of note.
 *
 * WARNINGS
 *
 * A single warning can be thrown by this function.
 * @warning null_object -- If either @param file or @param buffer is NULL,
 * this warning will be thrown and nothing will be done.
 *
 * ERRORS
 *
 * One error can be thrown by this function.
 * @exception file_write_failed -- If the write to the file fails, this
 * error is thrown and the process ends. This typically means the file
 * handler is corrupt.
 *
 */
void LetoWriteFile(file_t* file, uint8_t* buffer, size_t buffer_size);

#endif // __LETO__FILES__
