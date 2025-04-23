#include "core/blx_logger.h"
#include "test_manager.h"
#include "tests/test_str.h"
#include "tests/test_list.h"
#include "tests/test_hashTable.h"
#include "tests/test_linkedList.h"

int main(void)
{
    testManager_Init();
    
    testStr_RegisterTests();
    testList_RegisterTests();
    testHashTable_RegisterTests();
    testLinkedList_RegisterTests();

    testManager_RunTests();

    return 0;
}