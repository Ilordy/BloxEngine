#pragma once
#include "core/blx_defines.h"

namespace BlxStl
{
    class BLXAPI AtomicInt32
    {
      private:
        volatile i32 value;

      public:
        AtomicInt32() : value(0) {}
        AtomicInt32(i32 initialValue) : value(initialValue) {}
        AtomicInt32(const AtomicInt32&) = delete;
        AtomicInt32& operator=(const AtomicInt32&) = delete;

        i32 Increment();
        i32 Decrement();
        i32 Add(i32 amount);
        i32 Subtract(i32 amount);
        void Set(i32 newValue);

        inline i32 Value() const
        {
            return value;
        }

        // Pre increment operator
        inline i32 operator++()
        {
            return Increment();
        }

        // Post increment operator
        inline i32 operator++(int)
        {
            return Increment() - 1;
        }

        // Pre decrement operator
        inline i32 operator--()
        {
            return Decrement();
        }

        // Post decrement operator
        inline i32 operator--(int)
        {
            return Decrement() + 1;
        }

        inline i32 operator+=(i32 amount)
        {
            return Add(amount);
        }

        inline i32 operator-=(i32 amount)
        {
            return Subtract(amount);
        }

        inline AtomicInt32& operator=(i32 newValue)
        {
            Set(newValue);
            return *this;
        }

        inline bool operator==(i32 other) const
        {
            return Value() == other;
        }

        inline bool operator!=(i32 other) const
        {
            return Value() != other;
        }
    };
} // namespace BlxStl