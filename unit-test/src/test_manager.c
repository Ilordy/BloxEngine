#include "test_manager.h"
#include "core/blx_logger.h"
#include "utils/blx_vlist.h"


/// @brief List of tests to run (These are a list of type TestFunc).
static void* testList;

void testManager_Init()
{
    testList = blxInitList(TestFunc);
}

void testManager_AddTest(TestFunc func)
{
    blxAddValueToList(testList, func);
}

void testManager_RunTests()
{
    BLXDEBUG("Running tests...");
    uint64 numPassed = 0;
    uint64 numFailed = 0;
    uint64 totalTests = blxGetListCount(testList);

    for (uint64 i = 0; i < totalTests; i++)
    {
        TestFunc func = ((TestFunc*)testList)[i];

        if (func()) {
            numPassed++;
        }
        else {
            numFailed++;
        }
    }

    BLXDEBUG("All test finished.");
    BLXINFO("Tests passed: %d/%d", numPassed, totalTests);

    if (numFailed > 0)
    {
        BLXERROR("Tests failed: %d/%d", numFailed, totalTests);
    }
}
