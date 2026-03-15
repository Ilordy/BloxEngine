#pragma once
#include "AtomicInt32.h"

namespace BlxStl
{
    class BLXAPI AtomicBool
    {
      private:
        AtomicInt32 value;

      public:
        AtomicBool() : value(false) {}
        AtomicBool(bool initialValue) : value(initialValue) {}
        AtomicBool(const AtomicBool&) = delete;
        AtomicBool& operator=(const AtomicBool&) = delete;

        inline const bool Value() const
        {
            return value.Value();
        }

        inline void Set(bool newValue)
        {
            value.Set(newValue);
        }

        inline bool operator=(bool newValue)
        {
            Set(newValue);
            return newValue;
        }

        inline bool operator==(bool other) const
        {
            return Value() == other;
        }

        inline bool operator!=(bool other) const
        {
            return Value() != other;
        }

        inline operator bool() const
        {
            return Value();
        }
    };
} // namespace BlxStl