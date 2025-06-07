#include "core/blx_defines.h"

///@brief Structure used to manage a free list of memory blocks.
///@note It uses a linked list to keep track of free memory blocks and allows for coalescing adjacent free blocks.
typedef struct blxFreeList blxFreeList;

/// @brief Creates a free list to manage memory blocks.
/// @param memory Pointer to the memory region to be managed.
/// @param size Size of the memory region in bytes.
/// @return Pointer to the created free list.
BLXAPI blxFreeList* blxFreeList_Create(void* memory, uint64 size);

/// @brief Allocates a memory block of the specified size from the free list.
/// @param freeList Pointer to the free list.
/// @param size Size of the memory block to allocate in bytes.
/// @return Pointer to the allocated memory block, or NULL if allocation fails.
BLXAPI void* blxFreeList_GetMem(blxFreeList* freeList, uint64 size);

/// @brief Frees a previously allocated memory block and returns it to the free list.
/// @param freeList Pointer to the free list.
/// @param mem Pointer to the memory block to free.
BLXAPI void blxFreeList_FreeMem(blxFreeList* freeList, void* mem);

/// @brief Gets the total amount of free memory available in the free list.
/// @param freeList Pointer to the free list.
/// @return Total free memory in bytes.
BLXAPI uint64 blxFreeList_GetFreeMemCount(blxFreeList* freeList);

/// @brief Gets the total number of memory currently allocated from the free list.
/// @param freeList Pointer to the free list.
/// @return Number of memory blocks.
BLXAPI uint64 blxFreeList_GetMemCount(blxFreeList* freeList);

/// @brief Clears the free list and releases all associated resources.
/// @param freeList Pointer to the pointer of the free list to clear.
BLXAPI void blxFreeList_Clear(blxFreeList** freeList);

/// @brief Returns the minimum size the free list needs to be able to function.
/// @return Minimum size in bytes required for the free list.
BLXAPI uint64 blxFreeList_GetInternalSize();

/// @brief Gets the size of the memory node managed by the free list.
/// @param mem Pointer to the memory block managed by the free list.
/// @note The memory block must have been allocated from the free list!
/// @return Size of the memory block in bytes.
BLXAPI uint64 blxFreeList_GetMemSize(void* mem);