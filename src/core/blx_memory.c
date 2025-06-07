#include "blx_memory.h"
#include "blx_logger.h"
#include "internal/platform/platform.h"
#include "blx_memArena.h"
#include "utils/blx_assertions.h"
#include "blx_freeList.h"

// TEMP
#include <string.h>

typedef struct memStat {
    uint64 totalMemAllocated;
    uint64 taggedAllocations[_BLXMEMORY_MAX_TAGS];
}memStat;

static memStat stats;
static blxFreeList* freeList;

static const char* memStringTags[_BLXMEMORY_MAX_TAGS] = {
    "LIST",
    "LINKED LIST",
    "DICTIONARY",
    "STRING",
    "APPLICATION",
    "RENDERER",
    "TEXTURE",
    "GAME",
    "MATERIAL"
};

void _blxInitMemory()
{
    blxPlatform_MemSetZero(&stats, sizeof(stats));

    // TODO: The amount of memory to allocate should be able to be provided by the user as an option.
    uint64 memSize = GIBIBYTES(1) / 2;

    void* appMem = blxPlatform_Allocate(memSize);
    freeList = blxFreeList_Create(appMem, memSize);
}

void _blxShutDownMemory()
{

}

void* blxAllocate(uint64 size, blxMemoryTag tag)
{
    stats.totalMemAllocated += size;
    stats.taggedAllocations[tag] += size;

    return blxFreeList_GetMem(freeList, size);
}

void blxFree(void* block, blxMemoryTag tag)
{
    // Allow for null pointers to be "freed" to not have as much null checking in general code.
    if (!block) {
        return;
    }
    
    uint64 memSize = blxFreeList_GetMemSize(block);
    stats.totalMemAllocated -= memSize;
    stats.taggedAllocations[tag] -= memSize;

    blxFreeList_FreeMem(freeList, block);
}

void* blxMemCpy(void* dest, const void* src, uint64 size)
{
    return blxPlatform_MemCpy(dest, src, size);
}

void* blxZeroMemory(void* block, uint64 size)
{
    return blxPlatform_MemSetZero(block, size);
}

const char* blxGetMemoryUsage()
{
    const uint64 gib = 1024 * 1024 * 1024;
    const uint64 mib = 1024 * 1024;
    const uint64 kib = 1024;

    char buffer[8000] = "System memory use (tagged):\n";
    uint64 offset = strlen(buffer);

    for (unsigned int i = 0; i < _BLXMEMORY_MAX_TAGS; i++)
    {
        char unit[4] = "XiB";
        float amount = 1.0f;

        if (stats.taggedAllocations[i] >= gib) {
            unit[0] = 'G';
            amount = stats.taggedAllocations[i] / (float)gib;
        }
        else if (stats.taggedAllocations[i] >= mib) {
            unit[0] = 'M';
            amount = stats.taggedAllocations[i] / (float)mib;
        }
        else if (stats.taggedAllocations[i] >= kib) {
            unit[0] = 'K';
            amount = stats.taggedAllocations[i] / (float)kib;
        }
        else {
            unit[0] = 'B';
            unit[1] = '\0';
            amount = (float)stats.taggedAllocations[i];
        }

        //add to the beginning of buffer pointer by offset.
        unsigned int length = snprintf(buffer + offset, 8000, " %s: %.2f%s\n", memStringTags[i], amount, unit);
        offset += length;
    }
    char* outString = _strdup(buffer);
    return outString;
}

#pragma region Memory Arena
typedef struct blxMemoryArena
{
    void* base;
    uint64 size;
    uint64 used;
} blxMemoryArena;

void blxArena_Init(blxMemoryArena* arena, uint64 size)
{
    arena->base = blxPlatform_Allocate(size);
    arena->size = size;
    arena->used = 0;
}

void* blxArena_Alloc(blxMemoryArena* arena, uint64 size)
{
    BLXASSERT_MSG(arena->used + size <= arena->size, "Memory arena out of space!");

    void* block = (char*)arena->base + arena->used;
    arena->used += size;
    return block;
}

void blxArena_Clear(blxMemoryArena* arena)
{
    arena->used = 0;
}

void blxArena_Free(blxMemoryArena* arena)
{
    // TODO: UPDATE ARENAS TO USE FREELIST.
    blxPlatform_FreeMemory(arena->base);
}
#pragma endregion



