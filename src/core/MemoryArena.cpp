#include "MemoryArena.h"
#include "blx_defines.h"

BLXEXTERN_C_START
#include "core/blx_memory.h"
#include "utils/blx_assertions.h"
BLXEXTERN_C_END


namespace BlxMem
{
    Arena::Arena(u64 size) : size(size), offset(0)
    {
        head = (char*)blxAllocate(size, BLXMEMORY_TAG_ARENA);
    }

    Arena::~Arena()
    {
        blxFree(head, BLXMEMORY_TAG_ARENA);
    }

    void* Arena::Alloc(u64 size)
    {
        BLXASSERT(offset + size <= this->size);

        void* ptr = head + offset;
        offset += size;
        return ptr;
    }

} // namespace BlxMem