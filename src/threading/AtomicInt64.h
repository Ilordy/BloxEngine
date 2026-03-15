#pragma once
#include "core/blx_defines.h"

namespace BlxStl
{
    class BLXALIGN_8 BLXAPI AtomicInt64
    {
      private:
        volatile i64 value;

      public:
        AtomicInt64() : value(0) {}
        AtomicInt64(i64 initialValue) : value(initialValue) {}
        AtomicInt64(const AtomicInt64&) = delete;
        AtomicInt64& operator=(const AtomicInt64&) = delete;

        i64 Increment();
        i64 Decrement();
        i64 Add(i64 amount);
        i64 Subtract(i64 amount);
        void Set(i64 newValue);

        inline i64 Value() const
        {
            return value;
        }

        // Pre increment operator
        inline i64 operator++()
        {
            return Increment();
        }

        // Post increment operator
        inline i64 operator++(int)
        {
            return Increment() - 1;
        }

        // Pre decrement operator
        inline i64 operator--()
        {
            return Decrement();
        }

        // Post decrement operator
        inline i64 operator--(int)
        {
            return Decrement() + 1;
        }

        inline i64 operator+=(i64 amount)
        {
            return Add(amount);
        }

        inline i64 operator-=(i64 amount)
        {
            return Subtract(amount);
        }

        inline AtomicInt64& operator=(i64 newValue)
        {
            Set(newValue);
            return *this;
        }

        inline bool operator==(i64 other) const
        {
            return Value() == other;
        }

        inline bool operator!=(i64 other) const
        {
            return Value() != other;
        }
    };
} // namespace BlxStl