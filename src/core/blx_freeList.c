#include "blx_freeList.h"
#include "utils/blx_assertions.h"
#include "utils/blx_linkedList.h"
#include "core/blx_memory.h"

/// @brief A node in the free list used to keep track of free memory blocks.
typedef struct FreeListNode {
    /// @brief Pointer to the next node in the list
    struct FreeListNode* next;

    /// @brief Size of the memory block in bytes
    /// @note This size does not include the size of the FreeListNode itself.
    uint64 size;
} FreeListNode;

typedef struct blxFreeList {
    /// @brief Pointer to the memory block
    void* memory;

    /// @brief Total size of the memory block allocated for the free list.
    uint64 size;

    /// @brief Head of the linked list of free memory blocks.
    FreeListNode* head;
} blxFreeList;

#define NODE_SIZE sizeof(FreeListNode)

/// @brief Returns the start of the client memory block from the FreeListNode.
/// @note This is used to get the pointer to the memory block that the FreeListNode points to.
/// @param node Pointer to the FreeListNode
#define MemStart(node) ((char*)node + NODE_SIZE)

/// @brief Returns the end of the client memory block from the FreeListNode.
/// @param node Pointer to the FreeListNode
#define MemEnd(node) (MemStart(node) + node->size)

blxFreeList* blxFreeList_Create(void* memory, uint64 size)
{
    BLXASSERT(size > sizeof(blxFreeList) + NODE_SIZE);

    blxZeroMemory(memory, size);

    blxFreeList* freeList = memory;

    memory = (char*)memory + sizeof(blxFreeList);

    freeList->memory = memory;
    freeList->size = size - blxFreeList_GetInternalSize();

    freeList->head = memory;
    freeList->head->size = freeList->size;

    return freeList;
}

void* blxFreeList_GetMem(blxFreeList* freeList, uint64 size)
{
    BLXASSERT(size > 0 && size <= freeList->size);

    uint64 totalSize = size + NODE_SIZE;
    FreeListNode* currentNode = freeList->head;

    while (currentNode != NULL) {

        // If the current node is exactly the size we need, return it.
        if (currentNode->size == size) {

            if (currentNode == freeList->head) {
                // Move the head to the next node
                freeList->head = currentNode->next;
            }

            // Return the pointer to the memory block
            return MemStart(currentNode);
        }

        // Else if our current node is larger than the size we need, we can split it.
        else if (currentNode->size > totalSize) {

           // The new node will be split from the current node
           // Make the new node at the end of the current node's memory block minus the size of the new node.
            FreeListNode* newNode = MemEnd(currentNode) - totalSize;

            newNode->size = size;
            currentNode->size -= totalSize;

            // Return the pointer to the memory block
            return MemStart(newNode);
        }

        currentNode = currentNode->next;
    }

    // TODO: Decide if we want to allocate more memory or not.
    // TODO: This should be implemented by the user itself.

    BLXASSERT_MSG(currentNode != NULL,
        "No suitable block found in the free list. Might be out of memory.");

    return NULL;
}

void blxFreeList_FreeMem(blxFreeList* freeList, void* mem)
{
    BLXASSERT_MSG((char*)mem >= (char*)freeList->memory + sizeof(blxFreeList) &&
        (char*)mem < (char*)freeList->memory + sizeof(blxFreeList) + freeList->size,
        "Invalid Memory Pointer given. Memory pointer must be in the range of the free list memory block.");

    FreeListNode* node = (char*)mem - NODE_SIZE;
    BLXASSERT(node != NULL);

    FreeListNode* current = freeList->head;
    FreeListNode* prev = NULL;

    // Place the node in the free list in sorted order.
    while (current && current < node)
    {
        prev = current;
        current = current->next;
    }

    // Next node is the node right after the current node.
    // This is used to coalesce the node with the next node if possible.
    node->next = current;

    if (prev)
    {
        // Prev is the node right before the current node.
        // This is used to coalesce the node with the previous node if possible.
        prev->next = node;
    }
    else
    {
        freeList->head = node;
    }

    // coalece with next node if possible
    if (node->next && (char*)node + node->size + NODE_SIZE == (char*)node->next)
    {
        node->size += node->next->size + NODE_SIZE;
        node->next = node->next->next;
    }

    // coalesce with previous node if possible
    if (prev->next && (char*)prev + prev->size + NODE_SIZE == (char*)node)
    {
        prev->size += node->size + NODE_SIZE;
        prev->next = node->next;
    }

    blxZeroMemory(mem, node->size);
}

uint64 blxFreeList_GetFreeMemCount(blxFreeList* freeList)
{
    // Loop through the free list and sum the sizes of all the nodes
    // to get the total amount of free memory in the free list.
    uint64 memCount = 0;
    FreeListNode* currentNode = freeList->head;
    while (currentNode) {
        memCount += currentNode->size;
        currentNode = currentNode->next;
    }

    return memCount;
}

uint64 blxFreeList_GetMemCount(blxFreeList* freeList) {
    return freeList->size - blxFreeList_GetFreeMemCount(freeList);
}

void blxFreeList_Clear(blxFreeList** freeList) {
    *freeList = blxFreeList_Create((*freeList)->memory, (*freeList)->size);
}

uint64 blxFreeList_GetInternalSize() {
    return sizeof(blxFreeList) + NODE_SIZE;
}

uint64 blxFreeList_GetMemSize(void* mem)
{
    BLXASSERT(mem != NULL);
    return ((FreeListNode*)((char*)mem - NODE_SIZE))->size;
}

