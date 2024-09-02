#pragma once
#include "core/blx_defines.h"
#include "blx_linkedList.h"
#include <vcruntime_string.h>

#define BLX_DEFAULT_HASH_TABLE_SIZE 11

// TODO: Add support for assertsions.
// TODO: Add support for memory arenas.

/// @brief Creates a new hash table with the default size.
/// @param keyType The type the hash table keys will be.
/// @param valueType The type the hash table values will be.
/// @param compareFunction Compare function pointer for comparing keys.
/// @returns A newly created hash table.
#define blxCreateHashTable(keyType, valueType, compareFunction) _blxCreateHashTable(sizeof(keyType), sizeof(valueType), BLX_DEFAULT_HASH_TABLE_SIZE, compareFunction)

/// @brief Creates a new hash table with given size.
/// @param keyType The type the hash table keys will be.
/// @param valueType The type the hash table values will be.
/// @param tableSize The size for the hash table.
/// @param compareFunction Compare function pointer for comparing keys.
/// @returns A newly created hash table with given size.
#define blxCreateHashTableWithSize(keyType, valueType, tableSize, compareFunction) _blxCreateHashTable(sizeof(keyType), sizeof(valueType), tableSize, compareFunction)

/// @brief Adds a given key value pair to given hash table.
/// @param table The table to add our key value pair to.
/// @param key The key to add (Can be a literal).
///@param value The value to add (Can be a literal).
#define blxAddToHashTable(table, keyToAdd, valueToAdd)\
{\
    typeof(keyToAdd) keyTarget = keyToAdd;\
    blxLinkedNode* target = _blxLinkedNodeFromHashKey(table, &keyTarget);\
    if(target)\
    {\
        blxHashTableEntry* entry = (blxHashTableEntry*)target->value;\
        if(!entry->inUse)\
        {\
            *((typeof(keyToAdd)*)entry->key) = keyToAdd;\
            *((typeof(valueToAdd)*)entry->value) = valueToAdd;\
            entry->inUse = BLX_TRUE;\
        }\
        else\
        {\
            printf("Hash Table Key already exist!\n");\
        }\
    }\
    else\
    {\
        typeof(keyToAdd)* keyPtr = (typeof(keyToAdd)*)malloc(sizeof(keyToAdd));\
        *keyPtr = keyToAdd;\
        typeof(valueToAdd)* valuePtr = (typeof(valueToAdd)*)malloc(sizeof(valueToAdd));\
        *valuePtr = valueToAdd;\
        _blxAddToHashTable(table, keyPtr, valuePtr);\
    }\
}\

/// @brief Deletes a key from the hash table.
/// @param table The table to delete the key from.
/// @param key The key to delete.
#define blxDeleteFromHashTable(table, key)\
{\
    typeof(key) tmp = key;\
    _blxDeleteFromHashTable(table, &key);\
}

typedef struct {
    void* key;
    void* value;
    blxBool inUse;
}blxHashTableEntry;

/// @brief Hash table structure for storing key value pairs.
typedef struct {
    blxLinkedNode** _buckets;
    size_t _keySize;
    size_t _valueSize;
    size_t tableArraySize;
    blxBool(*KeyCompare) (void* a, void* b);
}blxHashTable;

static size_t _blxToHash(void* key, size_t size);

/// @brief Checks if a certain key exists in the hash table.
/// @param table The hash table to search the key for.
/// @param key The key value we want to search for.
/// @param outValue The value of the given key if found (pass NULL if you do not care about the value).
/// @return True if the given key was found, false other wise.
static blxBool blxHashTableKeyExist(blxHashTable* table, void* key, void* outValue)
{
    if (!key)
    {
        // TODO: Print or assertions for debugging.
        return BLX_FALSE;
    }

    size_t index = _blxToHash(key, table->_keySize) % table->tableArraySize;
    blxLinkedNode* currentNode = table->_buckets[index];
    while (currentNode)
    {
        if (currentNode->value)
        {
            blxHashTableEntry* entry = ((blxHashTableEntry*)currentNode->value);
            if (entry->key && table->KeyCompare(entry->key, key))
            {
                if (outValue) {
                    memcpy(outValue, entry->value, sizeof(table->_valueSize));
                }

                return BLX_TRUE;
            }
        }

        currentNode = currentNode->next;
    }

    return BLX_FALSE;
}

/// @brief Frees all allocated memory from given table.
static void blxFreeHashTable(blxHashTable* table)
{
    for (size_t i = 0; i < table->tableArraySize; i++)
    {
        blxFreeLinkedList(table->_buckets[i], BLX_TRUE);
    }

    free(table->_buckets);
    free(table);
}

// djb2 hash algorithm.
// https://theartincode.stanis.me/008-djb2/
static size_t _blxToHash(void* key, size_t size)
{
    //Try regular char* version.
    const uint8_t* data = (const uint8_t*)key;
    uint32_t hash = 5381; // Initial hash value

    for (size_t i = 0; i < size; ++i) {
        hash = ((hash << 5) + hash) + data[i];
    }

    return hash;
}

static blxHashTable* _blxCreateHashTable(size_t keySize, size_t valueSize, size_t tableSize, blxBool(*Compare) (void* a, void* b))
{
    blxHashTable* ht = (blxHashTable*)malloc(sizeof(blxHashTable));
    ht->_keySize = keySize;
    ht->_valueSize = valueSize;
    ht->KeyCompare = Compare;
    ht->_buckets = (blxLinkedNode**)malloc(sizeof(blxLinkedNode*) * tableSize);
    for (size_t i = 0; i < tableSize; i++)
    {
        ht->_buckets[i] = blxCreateLinkedNode(NULL);
    }
    ht->tableArraySize = tableSize;
    return ht;
}

static blxLinkedNode* _blxLinkedNodeFromHashKey(blxHashTable* table, void* key)
{
    size_t index = _blxToHash(key, table->_keySize) % table->tableArraySize;
    blxLinkedNode* currentNode = table->_buckets[index];
    while (currentNode)
    {
        if (currentNode->value)
        {
            blxHashTableEntry* entry = (blxHashTableEntry*)currentNode->value;

            if (entry->key && table->KeyCompare(entry->key, key))
            {
                return currentNode;
            }
        }
        currentNode = currentNode->next;
    }

    return NULL;
}

static void _blxAddToHashTable(blxHashTable* table, void* key, void* value)
{
    size_t index = _blxToHash(key, table->_keySize) % table->tableArraySize;
    blxLinkedNode* head = table->_buckets[index];
    blxLinkedNode* currentNode = head;
    while (currentNode)
    {
        blxHashTableEntry* entry = (blxHashTableEntry*)currentNode->value;
        if (entry) {
            if (!entry->inUse)
            {
                entry->key = key;
                entry->value = key;
                entry->inUse = BLX_TRUE;
                return;
            }
        }

        currentNode = currentNode->next;
    }

    // If all linked nodes are in use then create a new one and append it to the end.
    blxAppendLinkedNodeLiteral(head, ((blxHashTableEntry) {key, value, BLX_TRUE}));
}

static void _blxDeleteFromHashTable(blxHashTable* table, void* key)
{
    blxLinkedNode* target = _blxLinkedNodeFromHashKey(table, key);
    if (!target) {
        printf("Hash table key to delete was not found!\n");
        return;
    }

    ((blxHashTableEntry*)target->value)->inUse = BLX_FALSE;
}

