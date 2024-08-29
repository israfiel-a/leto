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

FILE* OpenFile(file_mode_t mode, const char* path);
FILE* OpenFileF(file_mode_t mode, const char* path_format, ...);
void CloseFile(FILE* file);

long GetFileSize(FILE* file);

char* GetFileContents(FILE* file);
char* GetFileContentsP(const char* path);
char* GetFileContentsPF(const char* path_format, ...);

#endif // __LETO__FILES__
