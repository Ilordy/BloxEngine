#pragma once
#include "core/blx_defines.h"

#define BLX_EMPTY_STRING "\0"

BLXAPI void blxStrCpy(const char* dest, const char* src);
BLXAPI blxBool blxStrCmp(const char* a, const char* b);
BLXAPI uint64 blxStrLen(const char* str);
BLXAPI blxBool blxStrNullOrEmpty(const char* str);
BLXAPI void blxStrnCpy(char* dest, const char* src, uint64 count);
BLXAPI char* blxStrTrim(char* str);
BLXAPI int blxStrIndexOfChar(const char* str, char ch);
BLXAPI const char* blxStrFindSubStr(const char* str, const char* subStr);
BLXAPI int blxStrIndexOfSubStr(const char* src, const char* subStr);
BLXAPI int blxStrIndexOfLastChar(const char* src, const char ch);