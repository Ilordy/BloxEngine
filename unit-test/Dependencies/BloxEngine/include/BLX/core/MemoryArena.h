#pragma once
#include "blx_defines.h"

// TODO: Support alignment

namespace BlxMem
{
    /**
     * @brief A simple linear memory allocator that allocates when initalized and frees when destroyed.
     * @note This arena does not call the constructor/destructor for cpp types!
     That is up to the user to handle. This class is also not thread-safe and can be paired with a
     mutex or similar to make it thread-safe.
     *
     */
    class BLXAPI Arena
    {
      public:
        Arena(uint64 size);
        ~Arena();

        /**
         * @brief Allocates memory from the arena.
         *
         * @param size
         * @return void*
         */
        void* Alloc(uint64 size);

        /**
         * @brief Sets the pointer offset back to 0, effectively marking all allocated memory as available.
         *
         */
        inline void Clear()
        {
            offset = 0;
        }

      private:
        char* head;
        uint64 size;
        uint64 offset;
    };
} // namespace BlxMem
