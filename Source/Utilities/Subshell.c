// documentation forthcoming

#include "Subshell.h"
#include <stdlib.h>

int32_t RunCommand(const char* command)
{
    // no reporting cuz circular recursion
    int32_t sysreturn = system(command);
    if (sysreturn == -1 || !WIFEXITED(sysreturn)) exit(EXIT_FAILURE);

    int32_t return_value = WEXITSTATUS(sysreturn);
    if (return_value == 127) exit(EXIT_FAILURE);
    return return_value;
}

static uint8_t notify_check = 2;
bool CheckLibNotify(void)
{
    if (notify_check != 2) return notify_check;

    int32_t ret = RunCommand("command -v notify-send &>/dev/null");
    if (ret > 0)
    {
        notify_check = false;
        return notify_check;
    }
    notify_check = true;
    return notify_check;
}
