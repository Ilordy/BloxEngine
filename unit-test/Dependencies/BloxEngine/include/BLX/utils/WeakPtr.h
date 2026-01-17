#pragma once
#include "SharedPtr.h"
#include "core/blx_defines.h"
#include "core/blx_memory.h"

// TODO: Add atomics later on to the ControlBlock for thread safety

namespace BlxStl
{
    /**
     * @brief A smart pointer class that holds a non-owning reference to an object managed by SharedPtr.
     * The smart pointer must hold the same type T as the SharedPtr it is observing.
     * @tparam T The type of the object being observed.
     */
    template <typename T>
    class WeakPtr
    {
      private:
        T* ptr;
        // TODO: This tag is only TEMP till I update blxFree to support no tags.
        typename SharedPtr<T>::ControlBlock* control;
        blxMemoryTag tag;

        void Release()
        {
            if (!control)
            {
                return;
            }

            control->observerCount--;
            if (control->refCount == 0 && control->observerCount == 0)
            {
                blxFree(control, tag);
                control = nullptr;
            }
        }

      public:
        WeakPtr(const SharedPtr<T>* sharedPtr) : ptr(sharedPtr->ptr), control(sharedPtr->control), tag(sharedPtr->tag)
        {
            control->observerCount++;
        }

        WeakPtr(const SharedPtr<T>& sharedPtr) : ptr(sharedPtr.ptr), control(sharedPtr.control), tag(sharedPtr.tag)
        {
            control->observerCount++;
        }

        WeakPtr(const WeakPtr<T>& other) : ptr(other.ptr), control(other.control), tag(other.tag)
        {
            control->observerCount++;
        }

        WeakPtr(WeakPtr<T>&& other) : ptr(other.ptr), control(other.control), tag(other.tag)
        {
            other.ptr = nullptr;
            other.control = nullptr;
        }

        WeakPtr& operator=(const WeakPtr<T>& other)
        {
            if (this == &other)
            {
                return *this;
            }

            Release();

            ptr = other.ptr;
            tag = other.tag;
            control = other.control;
            control->observerCount++;

            return *this;
        }

        WeakPtr& operator=(WeakPtr<T>&& other)
        {
            if (this == &other)
            {
                return *this;
            }

            Release();

            ptr = other.ptr;
            tag = other.tag;
            control = other.control;

            other.ptr = nullptr;
            other.control = nullptr;

            return *this;
        }

        T* operator->()
        {
            return ptr;
        }

        T* operator*()
        {
            return ptr;
        }

        ~WeakPtr()
        {
            Release();
        }

        /**
         * @brief Retrieves the raw pointer managed by the WeakPtr.
         * @note The returned pointer may be nullptr if the managed object has been deleted so always check IsValid()
         * before using it.
         * @return T* A pointer to the managed object.
         */
        T* Get()
        {
            return ptr;
        }

        /**
         * @brief Retrieves the raw pointer managed by the WeakPtr.
         * @note The returned pointer may be nullptr if the managed object has been deleted so always check IsValid()
         * before using it.
         * @return T* A pointer to the managed object.
         */
        const T* Get() const
        {
            return ptr;
        }

        /**
         * @brief Checks if the WeakPtr is still observing a valid object.
         * @return True if the observed object is still valid (not-null), False otherwise (null).
         */
        const bool IsValid() const
        {
            return control->refCount > 0;
        }
    };
} // namespace BlxStl