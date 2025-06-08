#include "test_hashTable.h" 
#include "core/blx_defines.h" 
#include "utils/blx_hashTable.h"
#include "../test_manager.h" 
#include "../test_should.h"

// Compare function for integers.
static blxBool IntCmp(void* a, void* b) {
    return *((int*)a) == *((int*)b);
}

// Compare function for strings.
static blxBool StrKeyCompare(void* a, void* b) {
    return blxStr_Cmp((char*)a, (char*)b);
}

// Hashing function for strings.
static uint64 StrToHash(void* key) {
    return blxToHash(key, blxStr_Len(key));
}

blxBool testHashTable_DoTests()
{
    // Test creating a hash table with integer keys and string values.
    blxHashTable* intTable = blxCreateHashTable(int, const char*, IntCmp);

    // Test adding values to the hash table.
    blxAddToHashTableAlloc(intTable, 1, "Hello"); // Add key 1 with value "Hello".
    blxAddToHashTableAlloc(intTable, 2, "World"); // Add key 2 with value "World".
    blxAddToHashTableAlloc(intTable, 3, "Test"); // Add key 3 with value "Test".

    char strValue[24]; // Buffer to store retrieved string values.

    // Keys to test for existence.
    int key1 = 1; 
    int key2 = 2;
    int key3 = 3;

    // Check if key 1 exists and retrieve its value.
    testShould_BeTrue(blxHashTableKeyExist(intTable, &key1, strValue));
    testShould_StringEqual(strValue, "Hello"); // Assert that the value is "Hello".

    // Check if key 2 exists and retrieve its value.
    testShould_BeTrue(blxHashTableKeyExist(intTable, &key2, strValue));
    testShould_StringEqual(strValue, "World"); // Assert that the value is "World".

    // Check if key 3 exists and retrieve its value.
    testShould_BeTrue(blxHashTableKeyExist(intTable, &key3, strValue));
    testShould_StringEqual(strValue, "Test"); // Assert that the value is "Test".

    // Delete key 2 from the hash table.
    blxDeleteFromHashTable(intTable, 2);

    // Check that key 2 no longer exists in the hash table.
    testShould_BeFalse(blxHashTableKeyExist(intTable, &key2, NULL));

    blxFreeHashTable(intTable);

    // Test creating a hash table with string keys and integer values.
    blxHashTable* strTable = blxCreateHashTableWithHash(const char*, int, StrKeyCompare, StrToHash);

    char strKey1[] = "Test"; // First string key.
    char strKey2[24] = "Strings in action"; // Second string key.
    char strKey3[64] = "Super huge long testing string in action ready to be hashed"; // Third string key.

    int strValue1 = 1; // Value associated with the first string key.
    int strValue2 = 2; // Value associated with the second string key.
    int strValue3 = 3; // Value associated with the third string key.

    // Add the first key-value pair to the hash table.
    blxAddToHashTableAllocA(strTable, strKey1, strValue1);
    // Add the second key-value pair to the hash table.
    blxAddToHashTableAllocA(strTable, strKey2, strValue2);
    // Add the third key-value pair to the hash table.
    blxAddToHashTableAllocA(strTable, strKey3, strValue3);

    int intValOut; // Variable to store retrieved integer values.

    // Check if the first string key exists and retrieve its value.
    testShould_BeTrue(blxHashTableKeyExist(strTable, "Test", &intValOut));
    testShould_Be(intValOut, strValue1); // Assert that the value is 1.

    // Check if the second string key exists and retrieve its value.
    testShould_BeTrue(blxHashTableKeyExist(strTable, "Strings in action", &intValOut));
    testShould_Be(intValOut, strValue2); // Assert that the value is 2.

    // Check if the third string key exists and retrieve its value.
    testShould_BeTrue(blxHashTableKeyExist(strTable, "Super huge long testing string in action ready to be hashed", &intValOut));
    testShould_Be(intValOut, strValue3); // Assert that the value is 3.

    return BLX_TRUE;
}

void testHashTable_RegisterTests()
{
    testManager_AddTest(testHashTable_DoTests);
}