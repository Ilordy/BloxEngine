#pragma once
#include "core/blx_defines.h"

/// @brief Tags used to categorize memory allocations for tracking purposes.
typedef enum blxMemoryTag {
    /// @brief Memory used for lists.
    BLXMEMORY_TAG_LIST,
    /// @brief Memory used for linked lists.        
    BLXMEMORY_TAG_LINKEDLIST,
    /// @brief Memory used for dictionaries.
    BLXMEMORY_TAG_DICT,
    /// @brief Memory used for strings.  
    BLXMEMORY_TAG_STRING,
    /// @brief Memory used for application data.   
    BLXMEMORY_TAG_APPLICATION,
    /// @brief Memory used for renderer data.
    BLXMEMORY_TAG_RENDERER,
    /// @brief Memory used for textures.
    BLXMEMORY_TAG_TEXTURE,
    /// @brief Memory used for game data.
    BLXMEMORY_TAG_GAME,
    /// @brief Memory used for materials.
    BLXMEMORY_TAG_MATERIAL,
    /// @brief Memory used for physics data.
    BLXMEMORY_TAG_PHYSICS,

    _BLXMEMORY_MAX_TAGS
} blxMemoryTag;

/// @brief Allocates a block of memory.
/// @param size The size of the block to allocate.
/// @param tag Memory tag to associate with this allocation.
/// @return A pointer to the allocated memory.
BLXAPI void* blxAllocate(uint64 size, blxMemoryTag tag);

/// @brief Frees a previously allocated block of memory.
/// @param block Pointer to the memory block to free.
/// @param tag Memory tag associated with this allocation.
BLXAPI void blxFree(void* block, blxMemoryTag tag);

/// @brief Copies memory from one location to another.
/// @param dest Destination memory block.
/// @param src Source memory block.
/// @param size Number of bytes to copy.
/// @return A pointer to the destination memory block.
BLXAPI void* blxMemCpy(void* dest, const void* src, uint64 size);

/// @brief Fills a block of memory with zeros.
/// @param block Pointer to the memory block to zero out.
/// @param size The size of the block to zero.
/// @return A pointer to the zeroed memory block.
BLXAPI void* blxZeroMemory(void* block, uint64 size);

/// @brief Retrieves a string representation of current memory usage statistics.
/// @return A constant string containing memory usage details.
BLXAPI const char* blxGetMemoryUsage();

// TODO: Take these internal functions out of the public header.

/// @brief Initializes the memory subsystem. For internal use only.
BLXAPI void _blxInitMemory();

/// @brief Shuts down the memory subsystem. For internal use only.
BLXAPI void _blxShutDownMemory();



