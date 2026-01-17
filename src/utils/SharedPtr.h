#pragma once
#include "MemHelper.h"
#include "core/blx_defines.h"
#include "core/blx_memory.h"
#include "maths/MathHelper.h"
#include "utils/MemHelper.h"
#include "utils/blx_assertions.h"

// TODO: Add atomics later on to the ControlBlock for thread safety

namespace BlxStl
{
    template <typename T>
    class WeakPtr;

    /**
     * @brief A smart pointer class that manages the lifetime of an object using reference counting.
     * @details When the last SharedPtr pointing to an object is destroyed or reset, the object is
     * deleted.
     * @tparam T The type of the object being managed.
     */
    template <typename T>
    class SharedPtr
    {
        template <typename U>
        friend class WeakPtr;

      private:
        struct ControlBlock
        {
            uint64 refCount;
            uint64 observerCount;

            ControlBlock() : refCount(1), observerCount(0) {}
        };

        T* ptr;
        ControlBlock* control;
        // TODO: This tag is only TEMP till I update blxFree to support no tags.
        blxMemoryTag tag;

        void Release()
        {
            if (!ptr)
            {
                return;
            }

            control->refCount--;

            if (control->refCount == 0)
            {
                BlxMem::DestructAt(ptr);
                ptr = nullptr;

                if (control->observerCount == 0)
                {
                    blxFree(control, tag);
                    control = nullptr;
                }
            }
        }

      public:
        /**
         * @brief Construct a new Shared Ptr object
         * @details This class allocates heap memory amounting to the sizeof it's ControlBlock plus
         * the sizeof the object T.
         * @details The allocation is done in a single block for both the control block and the
         * object for efficiency.
         * @tparam Args The types of the arguments to forward to the constructor of T.
         * @param tag Memory tag to associate with this allocation.
         * @param args The arguments to forward to the constructor of T.
         */
        template <typename... Args>
        explicit SharedPtr(blxMemoryTag tag, Args&&... args) : tag(tag)
        {
            void* mem = blxAllocate((sizeof(T) + sizeof(ControlBlock)), tag);

            control = (ControlBlock*)mem;
            ptr = (T*)((char*)mem + sizeof(ControlBlock));

            BlxMem::ConstructAt(control);
            BlxMem::ConstructAt(ptr, std::forward<Args>(args)...);
        }

        SharedPtr(const SharedPtr& other) : ptr(other.ptr), control(other.control)
        {
            if (ptr)
            {
                ++control->refCount;
            }
        }

        SharedPtr(SharedPtr&& other) noexcept : ptr(other.ptr), control(other.control)
        {
            other.Release();
            other.ptr = nullptr;
            other.control = nullptr;
        }

        SharedPtr& operator=(SharedPtr&& other) noexcept
        {
            if (this == &other)
            {
                return *this;
            }

            BLXASSERT(other.ptr != nullptr);
            Release();

            ptr = other.ptr;
            control = other.control;

            other.Release();
            other.ptr = nullptr;
            other.control = nullptr;

            return *this;
        }

        SharedPtr& operator=(const SharedPtr& other)
        {
            if (this == &other)
            {
                return *this;
            }

            BLXASSERT(other.ptr != nullptr);
            Release();

            ptr = other.ptr;
            control = other.control;
            control->refCount++;

            return *this;
        }

        T& operator*()
        {
            BLXASSERT(ptr != nullptr);
            return *ptr;
        }

        ~SharedPtr()
        {
            Release();
        }

        inline bool IsValid() const
        {
            return ptr != nullptr;
        }

        inline T* Get()
        {
            return ptr;
        }

        inline const T* Get() const
        {
            return ptr;
        }

        /**
         * @brief Retrieves the current reference count. When the reference count reaches zero,
         *  the managed object is deleted.
         * @return const uint64 The current reference count.
         */
        inline const uint64 ReferenceCount() const
        {
            return control->refCount;
        }

        /**
         * @brief Retrieves the current observer count. The observer count represents the number of
         * WeakPtr instances observing this SharedPtr.
         * @return const uint64 The current observer count.
         */
        inline const uint64 ObserverCount() const
        {
            return control->observerCount;
        }
    };

} // namespace BlxStl