#pragma once
#include "core/blx_defines.h"

namespace BlxStl
{
    class BLXAPI Mutex
    {
      private:
        void* impl;

      public:
        Mutex();
        ~Mutex();

        void Lock();
        void Unlock();

        // Non-copyable and non-movable
        Mutex(const Mutex&) = delete;
        Mutex& operator=(const Mutex&) = delete;
        Mutex(Mutex&&) = delete;
        Mutex& operator=(Mutex&&) = delete;
    };
} // namespace BlxStl