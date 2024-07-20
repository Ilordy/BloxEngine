#include "blx_logger.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WHITE_COLOR  "\033[0m"
#define RED_COLOR "\e[1;31m"
#define YELLOW_COLOR "\e[1;33m"
#define GREEN_COLOR "\e[1;32m"
#define WHITE_BOLD_COLOR "\e[1;37m"

//TODO: TEMP FOR NOW TILL PLATFORM LAYER!---------------------
#include <Windows.h>
#include <winuser.h>

void blxInitalizeLogging()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}
//----------------------------

//https://blog.7linternational.com/how-to-create-some-colorful-logs-in-an-application-written-in-c-4bd2fea6f009
void blxLog(blxLogType logType, const char* message, ...)
{
    time_t t = time(NULL);
    struct tm* local_time = localtime(&t);
    char time_header[512];
    sprintf(
        time_header,
        "\e[4m%04d/%02d/%02d %02d:%02d:%02d\e[m - ",
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
        sprintf(finalMsg, "%s[%sINFO%s] %s\n\n",
            time_header,
            GREEN_COLOR,
            WHITE_COLOR,
            formattedMsg);
        printf("%s", finalMsg);
        break;

    case BLX_WARNING_LOG:
        sprintf(finalMsg, "%s[%sWARNING%s] %s\n\n",
            time_header,
            YELLOW_COLOR,
            WHITE_COLOR,
            formattedMsg);
        printf("%s", finalMsg);
        break;

    case BLX_ERROR_LOG:
        sprintf(finalMsg, "%s[%sERROR%s] %s\n\n",
            time_header,
            RED_COLOR,
            WHITE_COLOR,
            formattedMsg);
        printf("%s", finalMsg);
        break;

    case BLX_DEBUG_LOG:
        sprintf(finalMsg, "%s[%sDEBUG%s] %s\n\n",
            time_header,
            WHITE_BOLD_COLOR,
            WHITE_COLOR,
            formattedMsg);
        printf("%s", finalMsg);
        break;
    }


}
