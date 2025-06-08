#include "test_list.h"
#include "utils/blx_vlist.h"
#include "../test_manager.h"
#include "cglm/struct/vec3.h"
#include "../test_should.h"

blxBool testList_DoTests()
{
    vec3s* vec3List = blxInitList(vec3s);

    vec3s vec3 = { 1.0f, 2.0f, 3.0f };
    blxAddValueToList(vec3List, vec3);

    vec3 = (vec3s){ 5.0f, 8.0f, 2.0f };
    blxAddValueToList(vec3List, vec3);

    vec3 = (vec3s){ 9.0f, 2.0f, 1.0f };
    blxAddValueToList(vec3List, vec3);

    testShould_BeTrue(blxGetListCount(vec3List) == 3);

    testShould_BeTrue(glms_vec3_eqv(vec3List[0], (vec3s) { 1.0f, 2.0f, 3.0f }));

    testShould_BeTrue(glms_vec3_eqv(vec3List[1], (vec3s) { 5.0f, 8.0f, 2.0f }));

    testShould_BeTrue(glms_vec3_eqv(vec3List[2], (vec3s) { 9.0f, 2.0f, 1.0f }));

    blxRemoveFromListAt(vec3List, 1);
    testShould_BeTrue(blxGetListCount(vec3List) == 2);

    testShould_BeTrue(glms_vec3_eqv(vec3List[0], (vec3s) { 1.0f, 2.0f, 3.0f }));
    testShould_BeTrue(glms_vec3_eqv(vec3List[1], (vec3s) { 9.0f, 2.0f, 1.0f }));

    blxClearList(vec3List);
    testShould_BeTrue(blxGetListCount(vec3List) == 0);

    char** strList = blxInitList(char*);
    char* str1 = "Hello";
    char* str2 = "World";
    char* str3 = "Test Wordds";
    blxAddValueToList(strList, str1);
    blxAddValueToList(strList, str2);
    blxAddValueToList(strList, str3);

    testShould_BeTrue(blxGetListCount(strList) == 3);

    testShould_StringEqual(strList[0], "Hello");
    testShould_StringEqual(strList[1], "World");
    testShould_StringEqual(strList[2], "Test Wordds");

    typedef int int3[3];

    int** intPtrList = blxInitList(int3*);

    int3 a = { 1, 2, 3 };
    int3 b = { 4, 5, 6 };
    int3 c = { 7, 8, 9 };

    int3* intPtr = &a;
    int3* intPtr2 = &b;
    int3* intPtr3 = &c;

    blxAddBufferToList(intPtrList, &intPtr);
    blxAddBufferToList(intPtrList, &intPtr2);
    blxAddBufferToList(intPtrList, &intPtr3);

    testShould_Be(blxGetListCount(intPtrList), 3);

    testShould_Be(intPtrList[0][0], 1);
    testShould_Be(intPtrList[1][1], 5);
    testShould_Be(intPtrList[2][2], 9);


    return BLX_TRUE;
}

void testList_RegisterTests()
{
    testManager_AddTest(testList_DoTests);
}

