#include "core/blx_defines.h"

#ifdef BLXWIN32
#include "threading/Semaphore.h"
BLXEXTERN_C_START
#include "utils/blx_assertions.h"
BLXEXTERN_C_END
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