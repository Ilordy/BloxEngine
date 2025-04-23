#include "test_linkedList.h"
#include "../test_manager.h"
#include "../test_should.h"
#include "core/blx_defines.h" 
#include "utils/blx_linkedList.h"

blxBool IntCmp(void* a, void* b) {
    return *((int*)a) == *((int*)b);
}

blxBool testLinkedList_DoTests()
{
    blxLinkedNode* intHead;

    // Test creating a linked list with integer values (First value being 1).
    blxInitLinkedNode(intHead, 1);

    // Append nodes to the linked list.
    blxAppendLinkedNodeLiteral(intHead, 2);
    blxAppendLinkedNodeLiteral(intHead, 3);

    // Test the values in the linked list.
    testShould_Be(blxGetLinkedNodeValueAs(intHead, int), 1);
    testShould_Be(blxGetLinkedNodeValueAs(intHead->next, int), 2);
    testShould_Be(blxGetLinkedNodeValueAs(intHead->next->next, int), 3);

    // Test insertion at the head
    blxInsertLinkedNodeAtHead(&intHead, (void*)&(int){203});
    testShould_Be(blxGetLinkedNodeValueAs(intHead, int), 203); // Verify new head value
    testShould_Be(blxGetLinkedNodeValueAs(intHead->next, int), 1); // Verify old head is now second

    // Test finding a node
    blxLinkedNode* foundNode = blxFindLinkedNode(intHead, (void*)&(int) { 2 }, IntCmp);
    testShould_Be(blxGetLinkedNodeValueAs(foundNode, int), 2); // Verify found node value

    // Test insertion after a specific node
    blxInsertLinkedNodeAfterNode(intHead->next, (void*)&(int){99}); // Insert after the second node
    testShould_Be(blxGetLinkedNodeValueAs(intHead->next->next, int), 99); // Verify inserted node value
    testShould_Be(blxGetLinkedNodeValueAs(intHead->next->next->next, int), 2); // Verify next node is correct

    
    return BLX_TRUE;
}


void testLinkedList_RegisterTests()
{
    testManager_AddTest(testLinkedList_DoTests);
}