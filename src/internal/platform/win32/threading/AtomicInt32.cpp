#include "threading/AtomicInt32.h"

#ifdef BLXWIN32
#include <windows.h>
#include <winnt.h>

namespace BlxStl
{
    void AtomicInt32::Set(i32 newValue)
    {
        InterlockedExchange((volatile LONG*)&value, (LONG)newValue);
    }

    i32 AtomicInt32::Increment()
    {
        return InterlockedIncrement((volatile LONG*)&value);
    }

    i32 AtomicInt32::Decrement()
    {
        return InterlockedDecrement((volatile LONG*)&value);
    }

    i32 AtomicInt32::Add(i32 amount)
    {
        return InterlockedAdd((volatile LONG*)&value, amount);
    }

    i32 AtomicInt32::Subtract(i32 amount)
    {
        return InterlockedAdd((volatile LONG*)&value, -amount);
    }
} // namespace BlxStl

#endif