#include "blx_string.h"
//TODO: Create own string implementations!
#include <string.h>
#include <ctype.h>

void blxStrCpy(const char* dest, const char* src)
{
    strcpy(dest, src);
}

blxBool blxStrCmp(const char* a, const char* b)
{
    //Case sensitive.
    return strcmp(a, b) == 0;
}

uint64 blxStrLen(const char* str)
{
    return strlen(str);
}

blxBool blxStrNullOrEmpty(const char* str)
{
    return str && !str[0];
}

void blxStrnCpy(char* dest, const char* src, uint64 count)
{
    strncpy(dest, src, count);
    dest[count] = '\0';
}

char* blxStrTrim(char* str)
{
    char* d = str;
    do {
        while (isspace(*d)) {
            ++d;
        }
    } while (*str++ = *d++);
    return str;
}

const char* blxStrFindSubStr(const char* str, const char* subStr)
{
    return strstr(str, subStr);
}

int blxStrIndexOfChar(const char* str, char ch)
{
    char* pch = strchr(str, ch);

    if (pch) {
        return pch - str;
    }
    else {
        return -1;
    }

}

int blxStrIndexOfSubStr(const char* src, const char* subStr)
{
    const char* str = blxStrFindSubStr(src, subStr);

    if (str) {
        return str - src;
    }
    else {
        return -1;
    }
}

int blxStrIndexOfLastChar(const char* src, const char ch)
{
    char* pch = strrchr(src, ch);

    if (pch) {
        return pch - src;
    }
    else {
        return -1;
    }
}


