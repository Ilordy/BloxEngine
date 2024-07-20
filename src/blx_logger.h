#pragma once
#include "Defines.h"

#define BLX_INFO_LOG_ENALBED 1
#define BLX_WARNING_LOG_ENALBED 1
#define BLX_DEBUG_LOG_ENALBED 1

#ifndef BLX_DEBUG
#define BLX_DEBUG_LOG_ENALBED 0
#endif 

typedef enum {
    BLX_ERROR_LOG = 0,
    BLX_WARNING_LOG = 1,
    BLX_INFO_LOG = 2,
    BLX_DEBUG_LOG = 3
}blxLogType;

// TODO: TEMP FOR NOW! TILL PLATFORM LAYER IS OUT.
BLXAPI void blxInitalizeLogging();

BLXAPI void blxLog(blxLogType logType, const char* message, ...);

#define BLXERROR(message, ...) blxLog(BLX_ERROR_LOG, message, ##__VA_ARGS__)

#if BLX_INFO_LOG_ENALBED == 1
#define BLXINFO(message, ...) blxLog(BLX_INFO_LOG, message, ##__VA_ARGS__)
#else
#define BLXINFO(message, ...)
#endif

#if BLX_WARNING_LOG_ENALBED == 1
#define BLXWARNING(message, ...) blxLog(BLX_WARNING_LOG, message, ##__VA_ARGS__)
#else
#define BLXWARNING(message, ...)
#endif

#if BLX_DEBUG_LOG_ENALBED == 1
#define BLXDEBUG(message, ...) blxLog(BLX_DEBUG_LOG, message, ##__VA_ARGS__)
#else
#define BLXDEBUG(message, ...)
#endif