#pragma once
#include <vcruntime_string.h>

#include "core/blx_defines.h"

/// @brief Initalizes a linked node. Useful for creating a linked node from a literal.
/// @param linkedNodePtr This should be a blxLinkedNode pointer.
/// @param value literal value to give.
#define blxInitLinkedNode(linkedNodePtr, value)\
{\
    linkedNodePtr = (blxLinkedNode*)malloc(sizeof(blxLinkedNode));\
    typeof(value)* temp = (typeof(value)*)malloc(sizeof(value));\
    *temp = value;\
    result->value = temp;\
}

/// @brief Allocates memory to append a literal to the end of the list.
/// @param head The start of the linked list.
/// @param value The value to add to the linked list.
#define blxAppendLinkedNodeLiteral(head, value)\
{\
    typeof(value)* temp = (typeof(value)*)malloc(sizeof(value));\
    *temp = value;\
    blxAppendLinkedNode(head, temp);\
}

/// @brief Helper macro for auto casting and derefrencing the linked node value.
/// @param linkedNode The blxLinkedNode we want to retrieve the value from.
/// @param type The type to cast the value to (Pointers not needed).
#define blxGetLinkedNodeValueAs(linkedNode, type) *((type*)linkedNode->value)

struct blxLinkedNode {
    struct blxLinkedNode* next;
    void* value;
    size_t _valueSize;
};
/// @brief Structure for interacting with linked lists.
typedef struct blxLinkedNode blxLinkedNode;

/// @brief Allocates memory to create a linked node.
/// @param value Pointer to the value this linked node should have.
/// @return The newly created linked node.
static blxLinkedNode* blxCreateLinkedNode(void* value)
{
    blxLinkedNode* result = (blxLinkedNode*)malloc(sizeof(blxLinkedNode));
    result->value = value;
    result->next = NULL;
    return result;
}

/// @brief Appends a new node to the end of a linked list.
/// @param head The start of the list.
/// @param value Pointer to the value the new node should have.
/// @return The newly created node.
static blxLinkedNode* blxAppendLinkedNode(blxLinkedNode* head, void* value)
{
    blxLinkedNode* node = blxCreateLinkedNode(value);
    blxLinkedNode* currentNode = head;
    while (currentNode->next != NULL)
    {
        currentNode = currentNode->next;
    }
    currentNode->next = node;
    return node;
}

/// @brief Changes the current start of a linked list to be a different node.
/// @param head The current start of the list.
/// @param value Pointer to a value the new head should have.
static void blxInsertLinkedNodeAtHead(blxLinkedNode** head, void* value)
{
    blxLinkedNode* node = blxCreateLinkedNode(value);
    node->next = *head;
    *head = node;
}

/// @brief Finds the first node that Compare returns true to.
/// @param head The start of the linked list.
/// @param value The value to find.
/// @param Compare Function pointer which returns true or false for comparing each item in the list.
/// @return The linked node if found, or NULL if not found.
static blxLinkedNode* blxFindLinkedNode(blxLinkedNode* head, void* value, blxBool(*Compare) (void* a, void* b))
{
    blxLinkedNode* currentNode = head;
    while (currentNode != NULL)
    {
        if (Compare(value, currentNode->value))
        {
            return currentNode;
        }

        currentNode = currentNode->next;
    }

    return NULL;
}

/// @brief Inserts a new linked node after target.
/// @param target The target linked node to insert our new node after.
/// @param value Pointer to the value our new linked node should have.
/// @return The newly created linked node.
static blxLinkedNode* blxInsertLinkedNodeAfterNode(blxLinkedNode* target, void* value)
{
    blxLinkedNode* node = blxCreateLinkedNode(value);
    node->next = target->next;
    target->next = node;
    return node;
}

// TODO: Finialize this function.
static blxBool blxIsPartOfLinkedList(blxLinkedNode* head, void* value)
{
    blxLinkedNode* temp = head;
    while (temp != NULL)
    {
        if (temp->value == value)
        {
            return BLX_TRUE;
        }
        temp = temp->next;
    }
    return BLX_FALSE;
}

/// @brief Frees all allocated memory in the linked list.
/// @param head The start of the linked list.
/// @param freeValue Set to true to also free the allocated memory of the linked node's value.
static void blxFreeLinkedList(blxLinkedNode* head, blxBool freeValue)
{
    while (head != NULL)
    {
        blxLinkedNode* currentNode = head;
        head = currentNode->next;
        if (freeValue)
        {
            free(currentNode->value);
        }
        free(currentNode);
    }
}
