#include "blx_string.h"
//TODO: Create own string implementations!
#include <string.h>

void blxStrCpy(const char* dest, const char* src)
{
    strcpy(dest, src);
}

blxBool blxStringCompare(const char* a, const char* b)
{
    //Case sensitive.
    return strcmp(a, b) == 0;
}

uint64 blxStrLen(const char* str)
{
    return strlen(str);
}
