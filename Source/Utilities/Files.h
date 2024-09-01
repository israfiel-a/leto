// documentation forthcoming

#ifndef __LETO__FILES__
#define __LETO__FILES__

#include <stdio.h>

typedef enum
{
    r,
    w,
    a,
    rw,
    ra,
} file_mode_t;

FILE* LetoOpenFile(file_mode_t mode, const char* path);
FILE* LetoOpenFileF(file_mode_t mode, const char* path_format, ...);
void LetoCloseFile(FILE* file);

size_t LetoGetFileSize(FILE* file);

char* LetoGetFileContents(FILE* file);
char* LetoGetFileContentsP(const char* path);
char* LetoGetFileContentsPF(const char* path_format, ...);

#endif // __LETO__FILES__
