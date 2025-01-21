#include "blx_hashTable.h"
#include "blx_linkedList.h"

typedef struct
{
    blxLinkedNode** _buckets;
    unsigned int _count;
}blxHashSet;


#define blxCreateHashSet(valueType)

blxBool IntCompare(void* a, void* b)
{
    return *((int*)a) == *((int*)b);
}

static void _blxCreateHashSet()
{
    blxHashTable* table = blxCreateHashTable(int, void*, IntCompare);

}