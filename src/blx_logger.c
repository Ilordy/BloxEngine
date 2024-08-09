#include "blx_logger.h"
#include "internal/platform/platform.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

// //TODO: TEMP FOR NOW TILL PLATFORM LAYER!---------------------
// #include <Windows.h>
// #include <winuser.h>

void blxInitalizeLogging()
{
    //TODO: INIT LOG FILE!
}
// //----------------------------

//https://blog.7linternational.com/how-to-create-some-colorful-logs-in-an-application-written-in-c-4bd2fea6f009
void blxLog(blxLogType logType, const char* message, ...)
{
    time_t t = time(NULL);
    struct tm* local_time = localtime(&t);
    char time_header[512];
    sprintf(
        time_header,
        "%04d/%02d/%02d %02d:%02d:%02d - ",
        local_time->tm_year + 1900,
        local_time->tm_mon + 1,
        local_time->tm_mday,
        local_time->tm_hour,
        local_time->tm_min,
        local_time->tm_sec
    );

    // TODO: Memory arenas must be used for logging!
    char formattedMsg[32000]; //Temp

    __builtin_va_list arg_ptr;
    va_start(arg_ptr, message);
    vsnprintf(formattedMsg, 32000, message, arg_ptr);
    va_end(arg_ptr);

    char finalMsg[32000];
    switch (logType)
    {
        case BLX_INFO_LOG:
            sprintf(finalMsg, "%s[INFO] %s\n\n", time_header, formattedMsg);
            PlatformWriteToConsole(finalMsg, BLX_INFO_LOG);
            break;

        case BLX_WARNING_LOG:
            sprintf(finalMsg, "%s[WARNING] %s\n\n", time_header, formattedMsg);
            PlatformWriteToConsole(finalMsg, BLX_WARNING_LOG);
            break;

        case BLX_ERROR_LOG:
            sprintf(finalMsg, "%s[ERROR] %s\n\n", time_header, formattedMsg);
            PlatformWriteToConsole(finalMsg, BLX_ERROR_LOG);
            break;

        case BLX_DEBUG_LOG:
            sprintf(finalMsg, "%s[DEBUG] %s\n\n", time_header, formattedMsg);
            PlatformWriteToConsole(finalMsg, BLX_DEBUG_LOG);
            break;
    }
}
