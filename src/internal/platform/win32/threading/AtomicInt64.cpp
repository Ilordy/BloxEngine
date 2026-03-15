#include "threading/AtomicInt64.h"

#ifdef BLXWIN32
#include <windows.h>
#include <winnt.h>

namespace BlxStl
{
    void AtomicInt64::Set(i64 newValue)
    {
        InterlockedExchange64((volatile LONG64*)&value, newValue);
    }

    i64 AtomicInt64::Increment()
    {
        return InterlockedIncrement64((volatile LONG64*)&value);
    }

    i64 AtomicInt64::Decrement()
    {
        return InterlockedDecrement64((volatile LONG64*)&value);
    }

    i64 AtomicInt64::Add(i64 amount)
    {
        return InterlockedAdd64((volatile LONG64*)&value, amount);
    }

    i64 AtomicInt64::Subtract(i64 amount)
    {
        return InterlockedAdd64((volatile LONG64*)&value, -amount);
    }
} // namespace BlxStl

#endif