#include "test_freeList.h"
#include "../test_manager.h" 
#include "../test_should.h"
#include "core/blx_freeList.h"
#include "core/blx_defines.h"
#include "cglm/vec3.h"

blxBool testFreeList_DoTests()
{
    // Allocate memory for the free list
    uint64 size = 1024 + blxFreeList_GetInternalSize();
    void* memory = malloc(size);
    blxFreeList* freeList = blxFreeList_Create(memory, size);

    testShould_BeFalse(freeList == NULL);
    testShould_Be(blxFreeList_GetMemCount(freeList), 0);
    testShould_Be(blxFreeList_GetFreeMemCount(freeList), size - blxFreeList_GetInternalSize());

    char* testStr = blxFreeList_GetMem(freeList, 100);
    blxStr_Cpy(testStr, "Hello World! From the FreeList!");

    testShould_StringEqual(testStr, "Hello World! From the FreeList!");

    vec3* testVec = blxFreeList_GetMem(freeList, sizeof(vec3));
    (*testVec)[0] = 1.0f;
    (*testVec)[1] = 2.0f;
    (*testVec)[2] = 3.0f;
    testShould_BeTrue(glm_vec3_eqv(*testVec, (vec3) { 1.0f, 2.0f, 3.0f }));

    int* testInt = blxFreeList_GetMem(freeList, sizeof(int) * 10);
    for (int i = 0; i < 10; i++) {
        testInt[i] = i;
    }

    for (int i = 0; i < 10; i++) {
        testShould_Be(testInt[i], i);
    }

    uint64 testSizeCount = sizeof(int) * 10 + sizeof(vec3) + 100;
    testShould_BeTrue(blxFreeList_GetMemCount(freeList) >= testSizeCount);
    testShould_BeTrue(blxFreeList_GetFreeMemCount(freeList) > testSizeCount && blxFreeList_GetFreeMemCount(freeList) < size);

    blxFreeList_FreeMem(freeList, testInt);
    blxFreeList_FreeMem(freeList, testStr);
    blxFreeList_FreeMem(freeList, testVec);

    testShould_Be(blxFreeList_GetFreeMemCount(freeList), size - blxFreeList_GetInternalSize());

    return BLX_TRUE;
}

void testFreeList_RegisterTests()
{
    testManager_AddTest(testFreeList_DoTests);
}
