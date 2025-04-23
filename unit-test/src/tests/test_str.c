#include "test_str.h"
#include "../test_should.h"
#include "core/blx_defines.h"
#include "../test_manager.h"
#include "core/blx_string.h"

blxBool testStr_DoTests()
{
    const char str1[36];
    const char* str2 = "Hello World!";

    // Test blxStrCpy
    blxStr_Cpy(str1, str2);
    testShould_StringEqual(str1, str2);

    // Test blxStrLen
    testShould_Be(blxStr_Len(str1), 12);

    //Test blxStrNullOrEmpty
    const char* str3 = BLX_EMPTY_STRING;
    testShould_BeTrue(blxStr_NullOrEmpty(str3));
    testShould_BeFalse(blxStr_NullOrEmpty(str1));

    // Test blxStrnCpy
    char str4[6];
    blxStrn_Cpy(str4, str2, 5);
    testShould_StringEqual(str4, "Hello");

    // Test blxStrTrim
    char str5[] = "      Trim me!      ";
    char* trimmed = blxStr_Trim(str5);
    testShould_StringEqual(trimmed, "Trim me!");

    // Test blxStrTrimAll
    char str6[] = "  Remove all spaces  ";
    char* trimmedAll = blxStr_TrimAll(str6);
    testShould_StringEqual(trimmedAll, "Removeallspaces");

    // Test blxStrIndexOfChar
    testShould_Be(blxStr_IndexOfChar(str2, 'W'), 6);
    testShould_Be(blxStr_IndexOfChar(str2, 'z'), -1);

    // Test blxStrFindSubStr
    const char* subStr = blxStr_FindSubStr(str2, "World");
    testShould_StringEqual(subStr, "World!");

    // Test blxStrIndexOfSubStr
    testShould_Be(blxStr_IndexOfSubStr(str2, "World"), 6);
    testShould_Be(blxStr_IndexOfSubStr(str2, "NotFound"), -1);

    // Test blxStrIndexOfLastChar
    testShould_Be(blxStr_IndexOfLastChar(str2, 'l'), 9);
    testShould_Be(blxStr_IndexOfLastChar(str2, 'z'), -1);

    return BLX_TRUE;
}   

void testStr_RegisterTests()
{
    testManager_AddTest(testStr_DoTests);
}