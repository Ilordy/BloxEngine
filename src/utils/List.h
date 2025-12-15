#pragma once
#include "core/blx_defines.h"

BLXEXTERN_C_START
#include "core/blx_memory.h"
#include "utils/blx_assertions.h"
BLXEXTERN_C_END

#include "utils/MemHelper.h"
#include <utility>

#ifndef BLX_LIST_GROWTH_FACTOR
#define BLX_LIST_GROWTH_FACTOR 2
#endif

namespace BlxStl
{
    /// @brief A simple dynamic array list implementation.
    /// @tparam T The type of elements stored in the list.
    template <typename T>
    class List
    {
      private:
        /// @brief The underlying array storing the elements.
        T* data;
        /// @brief The current allocated size of the array.
        uint64 size;
        /// @brief The current number of elements in the list.
        uint64 count;

        void ReAlloc(uint64 newSize)
        {
            T* newData = (T*)blxAllocate(sizeof(T) * newSize, BLXMEMORY_TAG_LIST);
            for (uint64 i = 0; i < count; i++)
            {
                // Calls move assignment operator if available,
                BlxMem::ConstructAt(&newData[i], std::move(data[i]));
            }

            BlxMem::DestructRange(data, data + count);
            blxFree((void*)data, BLXMEMORY_TAG_LIST);

            data = newData;
            size = newSize;
        }

      public:
        /// @brief Constructor to create a list with an optional reserved size.
        /// @param reservedSize The initial reserved size of the list.
        List(uint64 reservedSize = 2) : size(reservedSize), count(0)
        {
            data = (T*)blxAllocate(sizeof(T) * reservedSize, BLXMEMORY_TAG_LIST);
        }

        /// @brief Copy constructor to create a list as a copy of another.
        List(const List& other) : size(other.size), count(other.count)
        {
            data = (T*)blxAllocate(sizeof(T) * size, BLXMEMORY_TAG_LIST);
            for (uint64 i = 0; i < count; i++)
            {
                BlxMem::ConstructAt(&data[i], other.data[i]);
            }
        }

        /// @brief Move constructor to create a list by taking ownership of another's resources.
        List(List&& other) noexcept : data(other.data), size(other.size), count(other.count)
        {
            other.data = nullptr;
            other.size = 0;
            other.count = 0;
        }

        /// @brief Copy assignment operator to assign one list to another.
        List& operator=(const List& other)
        {
            if (this != &other)
            {
                BlxMem::DestructRange(data, data + count);
                blxFree((void*)data, BLXMEMORY_TAG_LIST);

                size = other.size;
                count = other.count;
                data = (T*)blxAllocate(sizeof(T) * size, BLXMEMORY_TAG_LIST);
                for (uint64 i = 0; i < count; i++)
                {
                    BlxMem::ConstructAt(&data[i], other.data[i]);
                }
            }
            return *this;
        }

        /// @brief Move assignment operator to assign one list to another by taking ownership of resources.
        List& operator=(List&& other) noexcept
        {
            if (this != &other)
            {
                BlxMem::DestructRange(data, data + count);
                blxFree((void*)data, BLXMEMORY_TAG_LIST);

                data = other.data;
                size = other.size;
                count = other.count;

                other.data = nullptr;
                other.size = 0;
                other.count = 0;
            }
            return *this;
        }

        /// @brief Destructor to clean up allocated memory.
        ~List()
        {
            BlxMem::DestructRange(data, data + count);
            blxFree((void*)data, BLXMEMORY_TAG_LIST);
        }

        inline const T& operator[](uint64 index) const
        {
            BLXASSERT(index < count);
            return data[index];
        }

        inline T& operator[](uint64 index)
        {
            BLXASSERT(index < count);
            return data[index];
        }

        /// @brief Adds an element to the end of the list, resizing if necessary.
        /// @param element The element to add to the list.
        void Add(const T* element)
        {
            BLXASSERT(element != nullptr);
            if (count >= size)
            {
                ReAlloc(size * BLX_LIST_GROWTH_FACTOR);
            }

            BlxMem::ConstructAt(&data[count++], *element);
        }

        /// @brief Adds an element to the end of the list, resizing if necessary.
        /// @param element The element to add to the list.
        void Add(const T& element)
        {
            if (count >= size)
            {
                ReAlloc(size * BLX_LIST_GROWTH_FACTOR);
            }

            BlxMem::ConstructAt(&data[count++], element);
        }

        /// @brief Move version of Add to efficiently add temporary objects.
        /// @param element Element to be moved into the list.
        void Add(T&& element)
        {
            if (count >= size)
            {
                ReAlloc(size * BLX_LIST_GROWTH_FACTOR);
            }

            BlxMem::ConstructAt(&data[count++], std::move(element));
        }

        /// @brief Constructs an element in place at the end of the list.
        /// @tparam Args The types of arguments to forward to the constructor.
        /// @param args The arguments to forward to the constructor of T.
        template <typename... Args>
        void Emplace(Args&&... args)
        {
            if (count >= size)
            {
                ReAlloc(size * BLX_LIST_GROWTH_FACTOR);
            }

            BlxMem::ConstructAt(&data[count++], std::forward<Args>(args)...);
        }

        /// @brief Gets the current number of elements in the list.
        inline uint64 Count() const
        {
            return count;
        }
    };
} // namespace BlxStl
