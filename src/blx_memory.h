#pragma once
#include "Defines.h"

typedef enum {
    BLXMEMORY_TAG_LIST,
    BLXMEMORY_TAG_LINKEDLIST,
    BLXMEMORY_TAG_DICT,
    BLXMEMORY_TAG_STRING,
    BLXMEMORY_TAG_APPLICATION,
    BLXMEMORY_TAG_RENDERER,
    BLXMEMORY_TAG_TEXTURE,
    BLXMEMORY_TAG_GAME,
    BLXMEMORY_TAG_MATERIAL,

    _BLXMEMORY_MAX_TAGS
}blxMemoryTag;

BLXAPI void* blxAllocate(unsigned long long size, blxMemoryTag tag);
BLXAPI void blxFree(void* block, unsigned long long size, blxMemoryTag tag);
BLXAPI void* blxMemCpy(void* dest, const void* src, uint64 size);
BLXAPI void* blxZeroMemory(void* block, uint64 size);
BLXAPI const char* blxGetMemoryUsage();

BLXAPI void _blxInitMemory();
BLXAPI void _blxShutDownMemory();


