#pragma once
#include "core/blx_defines.h"

BLXEXTERN_C_START
#include "core/blx_memory.h"
#include "utils/blx_assertions.h"
BLXEXTERN_C_END

#include <new>



// TODO: THIS SHOULD BE MOVED TO BLX_MEMORY ONCE CONVERTED TO CPP!

namespace BlxMem
{

    /// @brief Constructs an object of type T at the given memory location.
    /// @tparam T The type of object to construct.
    /// @param p Pointer to the memory location.
    /// @param ...args Constructor argument types.
    /// @note The memory location at p must be large enough to hold an object of type T.
    /// @return A pointer to the constructed object.
    template <typename T, typename... Args>
    inline static T* ConstructAt(T* p, Args&&... args)
    {
        return ::new (static_cast<void*>(p)) T(std::forward<Args>(args)...);
    }

    /// @brief Destroys an object by calling its destructor.
    /// @param p The pointer to the object to destroy.
    /// @note This does NOT free the memory; it only calls the destructor.
    template <typename T>
    inline static void DestructAt(T* p) noexcept
    {
        BLXASSERT(p != nullptr)
        p->~T();
    }

    /// @brief Destroys a range of objects by calling their destructors.
    /// @param first Pointer to the first object in the range.
    /// @param last Pointer to one past the last object in the range.
    template <typename T>
    static void DestructRange(T* first, T* last) noexcept
    {
        BLXASSERT(first != nullptr && last != nullptr)
        for (; first != last; ++first)
        {
            DestructAt(first);
        }
    }

    /// @brief Allocates memory and constructs an object of type T in that memory.
    /// @tparam T The type of object to create.
    /// @tparam ...Args The types of arguments to forward to the constructor.
    /// @param ...args The arguments to forward to the constructor of T.
    /// @param tag Memory tag to associate with this allocation.
    /// @return A pointer to the newly created object.
    template <typename T, typename... Args>
    static T* New(blxMemoryTag tag = BLXMEMORY_TAG_APPLICATION, Args&&... args)
    {
        void* mem = blxAllocate(sizeof(T), tag);
        return ConstructAt(static_cast<T*>(mem), std::forward<Args>(args)...);
    }

    template <typename T>
    static T* NewArray(uint64 count, blxMemoryTag tag = BLXMEMORY_TAG_APPLICATION)
    {
        T* mem = (T*)blxAllocate(sizeof(T) * count, tag);
        // value-initialize each element (calls default ctor or zero-inits POD)
        for (uint64 i = 0; i < count; ++i)
        {
            ConstructAt(&mem[i]);
        }
        return mem;
    }

    template <typename T>
    static void DeleteArray(T* arr, uint64 count, blxMemoryTag tag = BLXMEMORY_TAG_APPLICATION)
    {
        BLXASSERT(arr != nullptr)
        DestructRange(arr, arr + count);
        blxFree((void*)arr, tag);
    }

    /// @brief Destroys an object (calls its destructor) and frees its memory.
    /// @tparam T The type of the object to destroy.
    /// @param obj The object to destroy.
    /// @param tag Memory tag associated with this allocation.
    template <typename T>
    static void DestructAndFree(T* obj, blxMemoryTag tag = BLXMEMORY_TAG_APPLICATION)
    {
        BLXASSERT(obj != nullptr)
        DestructAt(obj);
        blxFree((void*)(obj), tag);
    }
} // namespace BlxMem