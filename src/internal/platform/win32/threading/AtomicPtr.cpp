#include "threading/AtomicPtr.h"

#ifdef BLXWIN32
#include <windows.h>

namespace BlxStl
{
    void _AtomicPtrImpl::SetInternal(void* newValue)
    {
        InterlockedExchangePointer((volatile PVOID*)&value, newValue);
    }
} // namespace BlxStl

#endif