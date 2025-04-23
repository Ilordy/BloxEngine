#pragma once
#include "core/blx_defines.h"
#include "blx_linkedList.h"
#include "core/blx_logger.h"
#include "core/blx_memory.h"

#include <vcruntime_string.h>

#define BLX_DEFAULT_HASH_TABLE_SIZE 97

// TODO: Add support for assertsions.
// TODO: Add support for memory arenas.
// TODO: Consider using a blx_vList instead of a linkedlist.
// TODO: Refactor

/// @brief Hashes a given key. (Not suited for arrays).
/// @param key The key to hash.
/// @param size The size of the key.
/// @return The hashed key.
static uint64 blxToHash(void* key, uint64 size);

/// @brief Creates a new hash table with the default size and hashing function (default hash does not work with arrays).
/// @param keyType The type the hash table keys will be.
/// @param valueType The type the hash table values will be.
/// @param compareFunction Compare function pointer for comparing keys.
/// @returns A newly created hash table.
#define blxCreateHashTable(keyType, valueType, compareFunction) _blxCreateHashTable(sizeof(keyType), sizeof(valueType), BLX_DEFAULT_HASH_TABLE_SIZE, compareFunction, NULL)

/// @brief Creates a new hash table with the default size.
/// @param keyType The type the hash table keys will be.
/// @param valueType The type the hash table values will be.
/// @param compareFunction Compare function pointer for comparing keys.
/// @param hashFunction Custom hashing function
/// @returns A newly created hash table.
#define blxCreateHashTableWithHash(keyType, valueType, compareFunction, hashFunction) _blxCreateHashTable(sizeof(keyType), sizeof(valueType), BLX_DEFAULT_HASH_TABLE_SIZE, compareFunction, hashFunction)

/// @brief Creates a new hash table with given size.
/// @param keyType The type the hash table keys will be.
/// @param valueType The type the hash table values will be.
/// @param tableSize The size for the hash table.
/// @param compareFunction Compare function pointer for comparing keys.
/// @param hashFunction Custom hashing function
/// @returns A newly created hash table with given size.
#define blxCreateHashTableWithSizeAndHash(keyType, valueType, tableSize, compareFunction, hashFunction) _blxCreateHashTable(sizeof(keyType), sizeof(valueType), tableSize, compareFunction, hashFunction)

/// @brief Creates a new hash table with given size and default hash function (default hash does not work with arrays).
/// @param keyType The type the hash table keys will be.
/// @param valueType The type the hash table values will be.
/// @param tableSize The size for the hash table.
/// @param compareFunction Compare function pointer for comparing keys.
/// @returns A newly created hash table with given size.
#define blxCreateHashTableWithSize(keyType, valueType, tableSize, compareFunction) _blxCreateHashTable(sizeof(keyType), sizeof(valueType), tableSize, compareFunction, NULL)

/// @brief Allocates memory to Add a given key value pair to given hash table
/// @brief Does not support literals (Use this function for static arrays).
/// @param table The table to add our key value pair to.
/// @param key The key to add.
/// @param value The value to add.
#define blxAddToHashTableAllocA(table, keyToAdd, valueToAdd)\
{\
    blxLinkedNode* target = _blxLinkedNodeFromHashKey(table, &keyToAdd);\
    typeof(keyToAdd)* keyPtr = (typeof(keyToAdd)*)malloc(sizeof(keyToAdd));\
    blxMemCpy(keyPtr, &keyToAdd, sizeof(keyToAdd));\
    typeof(valueToAdd)* valuePtr = (typeof(valueToAdd)*)malloc(sizeof(valueToAdd));\
    blxMemCpy(valuePtr, &valueToAdd, sizeof(valueToAdd));\
    if(target)\
    {\
        blxHashTableEntry* entry = (blxHashTableEntry*)target->value;\
        if(!entry->inUse)\
        {\
            entry->key = keyPtr;\
            entry->value = valuePtr;\
            entry->inUse = BLX_TRUE;\
        }\
        else\
        {\
            BLXWARNING("Hash Table Key already exist!\n");\
        }\
    }\
    else\
    {\
        blxAddToHashTable(table, keyPtr, valuePtr);\
    }\
}\

/// @brief Allocates memory to Add a given key value pair to given hash table.
/// @param table The table to add our key value pair to.
/// @param key The key to add (Can be a literal).
/// @param value The value to add (Can be a literal).
#define blxAddToHashTableAlloc(table, keyToAdd, valueToAdd)\
{\
    typeof(keyToAdd) keyTarget = keyToAdd;\
    typeof(valueToAdd) valueTarget = valueToAdd;\
    blxLinkedNode* target = _blxLinkedNodeFromHashKey(table, &keyTarget);\
    typeof(keyToAdd)* keyPtr = (typeof(keyToAdd)*)malloc(sizeof(keyToAdd));\
    blxMemCpy(keyPtr, &keyTarget, sizeof(keyToAdd));\
    typeof(valueToAdd)* valuePtr = (typeof(valueToAdd)*)malloc(sizeof(valueToAdd));\
    blxMemCpy(valuePtr, &valueTarget, sizeof(valueToAdd));\
    if(target)\
    {\
        blxHashTableEntry* entry = (blxHashTableEntry*)target->value;\
        if(!entry->inUse)\
        {\
            entry->key = keyPtr;\
            entry->value = valuePtr;\
            entry->inUse = BLX_TRUE;\
        }\
        else\
        {\
            BLXWARNING("Hash Table Key already exist!\n");\
        }\
    }\
    else\
    {\
        blxAddToHashTable(table, keyPtr, valuePtr);\
    }\
}\

/// @brief Deletes a key from the hash table.
/// @param table The table to delete the key from.
/// @param key The key to delete.
#define blxDeleteFromHashTable(table, key)\
{\
    typeof(key) tmp = key;\
    _blxDeleteFromHashTable(table, &tmp);\
}

#define _BLXTABLE_GET_BUCKET_INDEX(table, key, indexPtr) \
{\
    if(table->ToHash){\
        *indexPtr = table->ToHash(key) % table->tableArraySize;\
    }\
    else{\
        *indexPtr = blxToHash(key, table->_keySize) % table->tableArraySize;\
    }\
}\

// TODO: Add value size and key size here!
typedef struct {
    void* key;
    void* value;
    blxBool inUse;
}blxHashTableEntry;

/// @brief Hash table structure for storing key value pairs.
typedef struct {
    blxLinkedNode** _buckets;
    /// @brief Size of key type,
    ///does not take into account buffer sizes.
    uint64 _keySize;
    /// @brief Size of value type,
    ///does not take into account buffer sizes.
    uint64 _valueSize;
    uint64 tableArraySize;
    blxBool(*KeyCompare) (void* a, void* b);
    uint64(*ToHash) (void* key);
}blxHashTable;

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

    uint64 index;
    _BLXTABLE_GET_BUCKET_INDEX(table, key, &index);
    blxLinkedNode* currentNode = table->_buckets[index];
    while (currentNode)
    {
        if (currentNode->value)
        {
            blxHashTableEntry* entry = ((blxHashTableEntry*)currentNode->value);
            if (entry->key && entry->inUse && table->KeyCompare(entry->key, key))
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

/// @brief Adds a key value pair to the hash table. User is responsible for stack allocated memory.
/// @param table The table to add the kvp to.
/// @param key key to add.
/// @param value value to add.
static void blxAddToHashTable(blxHashTable* table, void* key, void* value)
{
    if (blxHashTableKeyExist(table, key, NULL)) {
        BLXWARNING("Hash table key already exist!");
        return;
    }

    uint64 index;
    _BLXTABLE_GET_BUCKET_INDEX(table, key, &index);
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

/// @brief Frees all allocated memory from given table.
static void blxFreeHashTable(blxHashTable* table)
{
    for (uint64 i = 0; i < table->tableArraySize; i++)
    {
        blxFreeLinkedList(table->_buckets[i], BLX_TRUE);
    }

    free(table->_buckets);
    free(table);
}

// djb2 hash algorithm.
// https://theartincode.stanis.me/008-djb2/
static uint64 blxToHash(void* key, uint64 size)
{
    //Try regular char* version.
    const uint8_t* data = (const uint8_t*)key;
    uint32_t hash = 5381; // Initial hash value

    for (uint64 i = 0; i < size; ++i) {
        hash = ((hash << 5) + hash) + data[i];
    }

    return hash;
}

static blxHashTable* _blxCreateHashTable(uint64 keySize, uint64 valueSize, uint64 tableSize,
    blxBool(*Compare) (void* a, void* b), uint64(*ToHash)(void* key))
{
    blxHashTable* ht = (blxHashTable*)malloc(sizeof(blxHashTable));
    ht->_keySize = keySize;
    ht->_valueSize = valueSize;
    ht->KeyCompare = Compare;
    ht->_buckets = (blxLinkedNode**)malloc(sizeof(blxLinkedNode*) * tableSize);
    for (uint64 i = 0; i < tableSize; i++)
    {
        ht->_buckets[i] = blxCreateLinkedNode(NULL);
    }
    ht->tableArraySize = tableSize;
    ht->ToHash = ToHash;
    return ht;
}

static blxLinkedNode* _blxLinkedNodeFromHashKey(blxHashTable* table, const void* key)
{
    uint64 index;
    _BLXTABLE_GET_BUCKET_INDEX(table, key, &index);
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

static void _blxDeleteFromHashTable(blxHashTable* table, void* key)
{
    blxLinkedNode* target = _blxLinkedNodeFromHashKey(table, key);
    if (!target) {
        BLXWARNING("Hash table key to delete was not found!\n");
        return;
    }

    ((blxHashTableEntry*)target->value)->inUse = BLX_FALSE;
}

