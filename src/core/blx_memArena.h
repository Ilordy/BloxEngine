#pragma once
#include "core/blx_defines.h"
#include "core/blx_memory.h"

/// @brief Memory arena struct.
typedef struct blxMemoryArena blxMemoryArena;

/// @brief Initializes a memory arena.
/// @param arena The arena to initialize.
/// @param size The amount of memory to reserve.
BLXAPI void blxArena_Init(blxMemoryArena* arena, uint64 size);

/// @brief Allocates memory from the arena.
/// @param arena The arena to allocate from.
/// @param size The size of the memory to allocate.
/// @return A pointer to the allocated memory.
BLXAPI void* blxArena_Alloc(blxMemoryArena* arena, uint64 size);

/// @brief Clears the memory from said arena without freeing it.
/// @param arena The arena to clear.
BLXAPI void blxArena_Clear(blxMemoryArena* arena);

/// @brief Frees the memory from the arena.
/// @param arena The arena whose memory to free.
BLXAPI void blxArena_Free(blxMemoryArena* arena);