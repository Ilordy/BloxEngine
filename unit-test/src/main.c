#include "core/blx_logger.h"
#include "test_manager.h"
#include "tests/test_str.h"
#include "tests/test_list.h"
#include "tests/test_hashTable.h"
#include "tests/test_linkedList.h"
#include "tests/test_freeList.h"
#include "tests/test_cpplist.h"
#include "tests/test_map.h"
#include "core/blx_memory.h"


int main(void)
{
    // Initializing memory purely for testing purposes.
    _blxInitMemory();

    testManager_Init();
    
    testStr_RegisterTests();
    testList_RegisterTests();
    testHashTable_RegisterTests();
    testLinkedList_RegisterTests();
    testFreeList_RegisterTests();
    testCppList_RegisterTests();
    testMap_RegisterTests();
    testManager_RunTests();

    return 0;
}