#pragma once
#include "Defines.h"
#include "blx_logger.h"

//Comment out to disable.
#define BLX_ASSERTIONS

#ifdef BLX_ASSERTIONS
#if _MSC_VER
#include <intrin.h>
#define debugBreak() __debugbreak()
#else
#define debugBreak() __builtin_trap()
#endif

#define BLXASSERT(expr)\
{\
    if(expr){\
    }\
    else{\
        BLXERROR("Assertion Failure: %s, in file: %s, line: %d\n", #expr, __FILE__, __LINE__);\
        debugBreak();\
    }\
}

#define BLXASSERT_MSG(expr, msg)\
{\
    if(expr){\
    }\
    else{\
        BLXERROR("Assertion Failure: %s, message: '%s', in file: %s, line: %d\n", #expr, msg, __FILE__, __LINE__);\
        debugBreak();\
    }\
}
#else
#define BLXASSERT(expr)
#define BLXASSERT_MSG(expr)
#endif