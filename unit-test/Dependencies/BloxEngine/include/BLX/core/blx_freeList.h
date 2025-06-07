#include "core/blx_defines.h"

///@brief Structure used to manage a free list of memory blocks.
///@note It uses a linked list to keep track of free memory blocks and allows for coalescing adjacent free blocks.
typedef struct blxFreeList blxFreeList;

BLXAPI blxFreeList* blxFreeList_Create(void* memory, uint64 size);

BLXAPI void* blxFreeList_GetMem(blxFreeList* freeList, uint64 size);

BLXAPI void blxFreeList_FreeMem(blxFreeList* freeList, void* mem);

BLXAPI uint64 blxFreeList_GetFreeMemCount(blxFreeList* freeList);

BLXAPI uint64 blxFreeList_GetMemCount(blxFreeList* freeList);

BLXAPI void blxFreeList_Clear(blxFreeList** freeList);

/// @brief Returns the minimum size the free list needs to be able to function.
BLXAPI uint64 blxFreeList_GetInternalSize();