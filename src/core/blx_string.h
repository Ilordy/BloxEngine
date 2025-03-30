#pragma once
#include "core/blx_defines.h"

/// @brief Empty string.
#define BLX_EMPTY_STRING "\0"

/// @brief Copies a string from one location to another.
/// @param dest The string to copy to.
/// @param src The string to copy from.
BLXAPI void blxStrCpy(const char* dest, const char* src);

/// @brief Compares two strings for equality.
/// @param a The first string to compare.
/// @param b The second string to compare.
/// @return True if the strings are equal, false otherwise.
BLXAPI blxBool blxStrCmp(const char* a, const char* b);

/// @brief Calculates the length of a string.
/// @param str The string whose length is to be calculated.
/// @return The length of the string.
BLXAPI uint64 blxStrLen(const char* str);

/// @brief Checks if a string is null or empty.
/// @param str The string to check.
/// @return True if the string is null or empty, false otherwise.
BLXAPI blxBool blxStrNullOrEmpty(const char* str);

/// @brief Copies a specified number of characters from one string to another.
/// @param dest The string to copy to.
/// @param src The string to copy from.
/// @param count The number of characters to copy.
BLXAPI void blxStrnCpy(char* dest, const char* src, uint64 count);

/// @brief Trims leading and trailing whitespace from a string.
/// @param str The string to trim.
/// @return A pointer to the trimmed string.
BLXAPI char* blxStrTrim(char* str);

/// @brief Finds the index of the first occurrence of a character in a string.
/// @param str The string to search.
/// @param ch The character to find.
/// @return The index of the character, or -1 if not found.
BLXAPI int blxStrIndexOfChar(const char* str, char ch);

/// @brief Finds the first occurrence of a substring in a string.
/// @param str The string to search.
/// @param subStr The substring to find.
/// @return A pointer to the first occurrence of the substring, or NULL if not found.
BLXAPI const char* blxStrFindSubStr(const char* str, const char* subStr);

/// @brief Finds the index of the first occurrence of a substring in a string.
/// @param src The string to search.
/// @param subStr The substring to find.
/// @return The index of the substring, or -1 if not found.
BLXAPI int blxStrIndexOfSubStr(const char* src, const char* subStr);

/// @brief Finds the index of the last occurrence of a character in a string.
/// @param src The string to search.
/// @param ch The character to find.
/// @return The index of the character, or -1 if not found.
BLXAPI int blxStrIndexOfLastChar(const char* src, const char ch);