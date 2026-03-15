#pragma once
#include "core/blx_defines.h"

namespace BlxStl
{
    class BLXAPI Semaphore
    {
      private:
        void* impl;

      public:
        /**
         * @brief Construct a new Semaphore object
         * @param initialCount The number of permits available at the start.
         * @param maxCount The maximum number of permits that can be released.
         */
        Semaphore(u32 initialCount, u32 maxCount);
        ~Semaphore();

        void Acquire();
        void Release(i32 releaseCount = 1);

        // Non-copyable and non-movable
        Semaphore(const Semaphore&) = delete;
        Semaphore& operator=(const Semaphore&) = delete;
        Semaphore(Semaphore&&) = delete;
        Semaphore& operator=(Semaphore&&) = delete;
    };
} // namespace BlxStl