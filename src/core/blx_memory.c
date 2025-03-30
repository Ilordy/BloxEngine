#include "blx_memory.h"
#include "blx_logger.h"
#include "internal/platform/platform.h"
#include "blx_memArena.h"
#include "utils/blx_assertions.h"

// TODO: custom string class
#include "string.h"

// TODO: Figure out if we'd like to also support memory stats for arenas. Arenas can also have their own individual memStats.
typedef struct memStat {
    uint64 totalMemAllocated;
    uint64 taggedAllocations[_BLXMEMORY_MAX_TAGS];
}memStat;

static memStat stats;

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
}

void _blxShutDownMemory()
{

}

void* blxAllocate(unsigned long long size, blxMemoryTag tag)
{
    stats.totalMemAllocated += size;
    stats.taggedAllocations[tag] += size;

    void* block = blxPlatform_Allocate(size);
    blxPlatform_MemSetZero(block, size);
    return block;
}

void blxFree(void* block, unsigned long long size, blxMemoryTag tag)
{
    stats.totalMemAllocated -= size;
    stats.taggedAllocations[tag] -= size;

    blxPlatform_FreeMemory(block);
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
    blxPlatform_FreeMemory(arena->base);
}
#pragma endregion



