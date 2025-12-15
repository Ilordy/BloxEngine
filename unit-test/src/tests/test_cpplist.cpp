#include "test_cpplist.h"

BLXEXTERN_C_START
#include "../test_manager.h"
#include "../test_should.h"
BLXEXTERN_C_END

#include "utils/List.h"

blxBool testCppList_DoTests()
{
    BlxStl::List<int> intList;

    for (int i = 0; i < 10; i++) {
        intList.Add(i * 2);
    }

    testShould_BeTrue(intList[0] == 0);
    testShould_BeTrue(intList[1] == 2);
    testShould_BeTrue(intList[2] == 4);
    testShould_BeTrue(intList[3] == 6);
    testShould_BeTrue(intList[4] == 8);
    testShould_BeTrue(intList[5] == 10);
    testShould_BeTrue(intList[6] == 12);
    testShould_BeTrue(intList[7] == 14);
    testShould_BeTrue(intList[8] == 16);
    testShould_BeTrue(intList[9] == 18);

    return BLX_TRUE;
}

void testCppList_RegisterTests()
{
    testManager_AddTest(testCppList_DoTests);
} 