#include "blx_memory.h"
#include "blx_logger.h"
#include "internal/platform/platform.h"

// TODO: custom string class
#include "string.h"

typedef struct {
    unsigned long long totalMemAllocated;
    unsigned long long taggedAllocations[_BLXMEMORY_MAX_TAGS];
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
    PlatformMemSetZero(&stats, sizeof(stats));
}

void _blxShutDownMemory()
{

}

void* blxAllocate(unsigned long long size, blxMemoryTag tag)
{
    stats.totalMemAllocated += size;
    stats.taggedAllocations[tag] += size;

    void* block = PlatformAllocate(size);
    PlatformMemSetZero(block, size);
    return block;
}

void blxFree(void* block, unsigned long long size, blxMemoryTag tag)
{
    stats.totalMemAllocated -= size;
    stats.taggedAllocations[tag] -= size;

    PlatformFreeMemory(block);
}

void* blxMemCpy(void* dest, const void* src, uint64 size)
{
    return PlatformMemCpy(dest, src, size);
}

void* blxZeroMemory(void* block, uint64 size)
{
    return PlatformMemSetZero(block, size);
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


