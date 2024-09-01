// documentation forthcoming
#include "Files.h"
#include <Output/Reporter.h>
#include <Utilities/Strings.h>

static FILE* OpenFile_(const char* mode, const char* path)
{
    FILE* opened_file = NULL;

#if defined(__LETO__LINUX__)
    opened_file = fopen(path, mode);
    if (opened_file == NULL) LetoReportError(file_open_failed);
#elif defined(__LETO__WINDOWS__)
    if (fopen_s(&opened_file, path, mode) != 0)
        LetoReportError(file_open_failed);
#endif

    return opened_file;
}

FILE* LetoOpenFile(file_mode_t mode, const char* path)
{
    if (path == NULL)
    {
        LetoReportWarning(null_string);
        return NULL;
    }

    FILE* opened_file = NULL;

    switch (mode)
    {
        case r:  opened_file = OpenFile_("rb", path); break;
        case w:  opened_file = OpenFile_("wb", path); break;
        case a:  opened_file = OpenFile_("ab", path); break;
        case rw: opened_file = OpenFile_("wb+", path); break;
        case ra: opened_file = OpenFile_("ab+", path); break;
        default:
            break; // Impossible, but some compilers throw a fit if not
                   // included.
    }

    return opened_file;
}

FILE* LetoOpenFileF(file_mode_t mode, const char* path_format, ...)
{
    if (path_format == NULL)
    {
        LetoReportWarning(null_string);
        return NULL;
    }

    va_list args;
    va_start(args, path_format);
    char* file_path = LetoStringCreateV(256, path_format, args);
    va_end(args);

    FILE* file = LetoOpenFile(mode, file_path);
    LetoStringFree(&file_path);
    return file;
}

void LetoCloseFile(FILE* file)
{
    if (file == NULL)
    {
        LetoReportWarning(preemptive_file_close);
        return;
    }

    if (fclose(file) == EOF) LetoReportError(file_close_failed);
}

size_t LetoGetFileSize(FILE* file)
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

char* LetoGetFileContents(FILE* file)
{
    if (file == NULL)
    {
        LetoReportWarning(null_object);
        return NULL;
    }

    size_t size = LetoGetFileSize(file);
    char* contents = LetoStringCalloc(size);
    if (fread(contents, 1, size, file) != size)
        LetoReportError(file_read_failed);

    return contents;
}

char* LetoGetFileContentsP(const char* path)
{
    if (path == NULL)
    {
        LetoReportWarning(null_string);
        return NULL;
    }

    FILE* file = LetoOpenFile(r, path);
    char* contents = LetoGetFileContents(file);
    LetoCloseFile(file);

    return contents;
}

char* LetoGetFileContentsPF(const char* path_format, ...)
{
    if (path_format == NULL)
    {
        LetoReportWarning(null_string);
        return NULL;
    }

    va_list args;
    va_start(args, path_format);

    char* path = LetoStringCreateV(256, path_format, args);
    FILE* file = LetoOpenFile(r, path);
    va_end(args);
    LetoStringFree(&path);

    char* contents = LetoGetFileContents(file);
    LetoCloseFile(file);
    return contents;
}
