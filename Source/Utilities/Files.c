// documentation forthcoming
#include "Files.h"
#include <Output/Reporter.h>
#include <Utilities/Strings.h>

FILE* OpenFile(file_mode_t mode, const char* path)
{
    if (path == NULL)
    {
        ReportWarning(null_string);
        return NULL;
    }

    FILE* opened_file = NULL;

    switch (mode)
    {
        case r:  opened_file = fopen(path, "rb"); break;
        case w:  opened_file = fopen(path, "wb"); break;
        case a:  opened_file = fopen(path, "ab"); break;
        case rw: opened_file = fopen(path, "wb+"); break;
        case ra: opened_file = fopen(path, "ab+"); break;
        default: break;
    }

    if (opened_file == NULL) ReportError(file_open_failed);
    return opened_file;
}

FILE* OpenFileF(file_mode_t mode, const char* path_format, ...)
{
    if (path_format == NULL)
    {
        ReportWarning(null_string);
        return NULL;
    }

    va_list args;
    va_start(args, path_format);
    char* file_path = StringCreateV(256, path_format, args);
    va_end(args);

    FILE* file = OpenFile(mode, file_path);
    StringFree(&file_path);
    return file;
}

void CloseFile(FILE* file)
{
    if (file == NULL)
    {
        ReportWarning(preemptive_file_close);
        return;
    }

    if (fclose(file) == EOF) ReportError(file_close_failed);
}

long GetFileSize(FILE* file)
{
    if (file == NULL)
    {
        ReportWarning(null_object);
        return 0;
    }

    if (fseek(file, 0L, SEEK_END) == -1)
        ReportError(file_positioner_set_failed);

    long size = ftell(file);
    if (size == -1) ReportError(file_positioner_get_failed);

    if (fseek(file, 0L, SEEK_SET) == -1)
        ReportError(file_positioner_set_failed);

    return size;
}

char* GetFileContents(FILE* file)
{
    if (file == NULL)
    {
        ReportWarning(null_object);
        return NULL;
    }

    long size = GetFileSize(file);
    char* contents = StringCalloc(size);
    if (fread(contents, 1, size, file) != size)
        ReportError(file_read_failed);

    return contents;
}

char* PathGetFileContents(const char* path)
{
    if (path == NULL)
    {
        ReportWarning(null_string);
        return NULL;
    }

    FILE* file = OpenFile(r, path);
    char* contents = GetFileContents(file);
    CloseFile(file);

    return contents;
}

char* PathGetFileContentsF(const char* path_format, ...)
{
    if (path_format == NULL)
    {
        ReportWarning(null_string);
        return NULL;
    }

    va_list args;
    va_start(args, path_format);

    char* path = StringCreateV(256, path_format, args);
    FILE* file = OpenFile(r, path);
    va_end(args);
    StringFree(&path);

    char* contents = GetFileContents(file);
    CloseFile(file);
    return contents;
}
