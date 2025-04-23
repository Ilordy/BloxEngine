#pragma once
#include "core/blx_defines.h"
#include "core/blx_logger.h"

#ifdef BLX_DEBUG
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
        BLXERROR("Assertion Failure: %s, message: '%s'\n, in file: %s, line: %d\n", #expr, msg, __FILE__, __LINE__);\
        debugBreak();\
    }\
}
#else
#define BLXASSERT(expr)
#define BLXASSERT_MSG(expr,)
#endif