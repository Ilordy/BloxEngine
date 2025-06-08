#include "blx_string.h"
//TODO: Create own string implementations!
#include <string.h>
#include <ctype.h>

/// @brief  Checks if a character is a whitespace character.
static blxBool charIsWhiteSpace(char charValue) {
    switch (charValue) {
        case ' ':  // regular space
        case '\n': // newline
        case '\r': // carriage return
        case '\f': // form feed
        case '\t': // horizontal tab
        case '\v': // vertical tab
            return BLX_TRUE;
        default:
            return BLX_FALSE;
    }
}

void blxStr_Cpy(const char* dest, const char* src)
{
    strcpy(dest, src);
}

blxBool blxStr_Cmp(const char* a, const char* b)
{
    //Case sensitive.
    return strcmp(a, b) == 0;
}

uint64 blxStr_Len(const char* str)
{
    return strlen(str);
}

blxBool blxStr_NullOrEmpty(const char* str)
{
    return str && !str[0];
}

void blxStrn_Cpy(char* dest, const char* src, uint64 count)
{
    strncpy(dest, src, count);
    dest[count] = '\0';
}

char* blxStr_Trim(char* str)
{
    while (charIsWhiteSpace(*str)) {
        str++;
    }
    if (*str) {
        char* p = str;
        while (*p) {
            p++;
        }
        while (charIsWhiteSpace(*(--p)));
        p[1] = '\0';
    }

    return str;
}

char* blxStr_TrimAll(char* str)
{
    int i, j = 0;
    for (i = 0; str[i] != '\0'; i++) {
        if (!charIsWhiteSpace(str[i])) {
            str[j++] = str[i];
        }
    }
    str[j] = '\0';
    return str;
}

const char* blxStr_FindSubStr(const char* str, const char* subStr)
{
    return strstr(str, subStr);
}

int blxStr_IndexOfChar(const char* str, char ch)
{
    char* pch = strchr(str, ch);

    if (pch) {
        return pch - str;
    }
    else {
        return -1;
    }

}

int blxStr_IndexOfSubStr(const char* src, const char* subStr)
{
    const char* str = blxStr_FindSubStr(src, subStr);

    if (str) {
        return str - src;
    }
    else {
        return -1;
    }
}

int blxStr_IndexOfLastChar(const char* src, const char ch)
{
    char* pch = strrchr(src, ch);

    if (pch) {
        return pch - src;
    }
    else {
        return -1;
    }
}


