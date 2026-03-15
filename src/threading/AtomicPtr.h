#include "core/blx_defines.h"

namespace BlxStl
{
    class _AtomicPtrImpl
    {
      private:
        volatile void* value;

      public:
        void SetInternal(void* newValue);
        inline void* ValueInternal() const
        {
            return (void*)value;
        }

        _AtomicPtrImpl() : value(nullptr) {}
        _AtomicPtrImpl(void* initialValue) : value(initialValue) {}
    };

    template <typename T>
    class BLXAPI AtomicPtr
    {
      private:
        _AtomicPtrImpl impl;

      public:
        AtomicPtr() : impl(nullptr) {}
        AtomicPtr(T* initialValue) : impl(initialValue) {}
        AtomicPtr(const AtomicPtr&) = delete;
        AtomicPtr& operator=(const AtomicPtr&) = delete;

        inline T* Value() const
        {
            return (T*)impl.ValueInternal();
        }

        inline void Set(T* newValue)
        {
            impl.SetInternal(newValue);
        }

        inline AtomicPtr& operator=(T* newValue)
        {
            Set(newValue);
            return *this;
        }

        inline bool operator==(T* other) const
        {
            return Value() == other;
        }

        inline bool operator!=(T* other) const
        {
            return Value() != other;
        }

        inline T& operator*() const
        {
            return *Value();
        }

        inline T& operator->() const
        {
            return *Value();
        }
    };
} // namespace BlxStl