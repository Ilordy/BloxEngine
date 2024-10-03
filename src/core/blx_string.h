#pragma once
#include "core/blx_defines.h"

BLXAPI void blxStrCpy(const char* dest, const char* src);
BLXAPI blxBool blxStringCompare(const char* a, const char* b);
BLXAPI uint64 blxStrLen(const char* str);