#include "core/blx_defines.h"

#ifdef BLXWIN32
#include "threading/Semaphore.h"
#include "utils/blx_assertions.h"
#include <windows.h>

namespace BlxStl
{
    Semaphore::Semaphore(u32 initialCount, u32 maxCount)
    {
        BLXASSERT_MSG(initialCount <= maxCount, "Initial count cannot be greater than max count.");
        impl = CreateSemaphore(nullptr, initialCount, maxCount, nullptr);
    }

    Semaphore::~Semaphore()
    {
        CloseHandle(impl);
    }

    void Semaphore::Acquire()
    {
        WaitForSingleObject(impl, INFINITE);
    }

    void Semaphore::Release(i32 releaseCount)
    {
        ReleaseSemaphore(impl, releaseCount, nullptr);
    }
} // namespace BlxStl
#endif