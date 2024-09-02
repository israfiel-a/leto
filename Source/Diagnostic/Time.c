#include "Time.h"
#include <Diagnostic/Platform.h>
#include <Output/Reporter.h>
#include <Utilities/Macros.h>
#include <Utilities/Strings.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#if defined(__LETO__LINUX__)
    #include <time.h>
#elif defined(__LETO__WINDOWS__)
    #define WIN32_LEAN_AND_MEAN
    #include <Windows.h>
    #include <sysinfoapi.h>
#endif

static uint64_t start_time = 0;

void LetoGetTimeRaw(uint64_t* ms)
{
#if defined(__LETO__LINUX__)
    struct timespec retrieved_time;
    int time_get_return = clock_gettime(CLOCK_MONOTONIC, &retrieved_time);
    if (time_get_return == -1) LetoReportError(time_get_error);

    if (start_time == 0)
    {
        start_time = NSEC_TO_MSEC(retrieved_time.tv_nsec);
        return;
    }
    *ms = NSEC_TO_MSEC(retrieved_time.tv_nsec) - start_time;
#elif defined(__LETO__WINDOWS__)
    FILETIME time_storage;
    GetSystemTimeAsFileTime(&time_storage);

    if (start_time == 0)
    {
        start_time = SYSTIME_TO_MS(time_storage);
        return;
    }
    *ms = SYSTIME_TO_MS(time_storage);
#endif
}

static void FormatTimeString_(timestamp_t* storage)
{
    if (storage == NULL) return;

    switch (storage->format)
    {
        case full:
            LetoSetStringF(
                false, &storage->string, TIMESTAMP_STRING_MAX_LENGTH,
                "%d milliseconds, %d seconds, %d minutes",
                storage->milliseconds, storage->seconds, storage->minutes);
            break;
        case shortened:
            LetoSetStringF(false, &storage->string,
                           TIMESTAMP_STRING_MAX_LENGTH, "%dms, %ds, %dm",
                           storage->milliseconds, storage->seconds,
                           storage->minutes);
            break;
        case bracketed:
            LetoSetStringF(false, &storage->string,
                           TIMESTAMP_STRING_MAX_LENGTH, "[%d:%d:%d]",
                           storage->milliseconds, storage->seconds,
                           storage->minutes);
            break;
        default: break;
    }
}

void LetoGetTimestamp(timestamp_t* storage, timestamp_format_t format)
{
    if (storage == NULL) return;

    uint64_t millisecond_count = 0;
    LetoGetTimeRaw(&millisecond_count);
    if (millisecond_count == 0)
    {
        *storage = TIMESTAMP_INITIALIZER;
        storage->format = format;
        FormatTimeString_(storage);
        return;
    }

    if (millisecond_count >= 1000)
    {
        storage->seconds = (uint32_t)(millisecond_count / 1000);
        storage->milliseconds = (uint32_t)(millisecond_count % 1000);
    }
    if (storage->seconds >= 60)
    {
        storage->minutes = storage->seconds / 60;
        storage->seconds = storage->seconds % 60;
    }
    storage->format = format;
    FormatTimeString_(storage);
}
