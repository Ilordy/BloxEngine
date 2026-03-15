#include "core/blx_defines.h"

#ifdef BLXWIN32
#include "threading/Mutex.h"

BLXEXTERN_C_START
#include "core/blx_memory.h"
#include "utils/blx_assertions.h"
BLXEXTERN_C_END

#include <windows.h>

namespace BlxStl
{
    Mutex::Mutex()
    {
        impl = blxAllocate(sizeof(SRWLOCK), BLXMEMORY_TAG_APPLICATION);
        InitializeSRWLock((SRWLOCK*)impl);
    }

    Mutex::~Mutex()
    {
        blxFree(impl, BLXMEMORY_TAG_APPLICATION);
    }

    void Mutex::Lock()
    {
        BLXASSERT(impl != nullptr);
        AcquireSRWLockExclusive((SRWLOCK*)impl);
    }

    void Mutex::Unlock()
    {
        BLXASSERT(impl != nullptr);
        ReleaseSRWLockExclusive((SRWLOCK*)impl);
    }
} // namespace BlxStl
#endif