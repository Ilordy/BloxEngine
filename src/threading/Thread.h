#pragma once

#include "core/blx_defines.h"

BLXEXTERN_C_START
#include "core/blx_memory.h"
#include "core/blx_string.h"
BLXEXTERN_C_END

#include "utils/MemHelper.h"

#include <tuple>
#include <type_traits>

namespace BlxStl
{
    class BLXAPI Thread
    {
      private:
        struct Impl;

        template <typename TFunc, typename... TArgs>
        struct ThreadData
        {
            void (*Run)(void*);

            TFunc func;
            std::tuple<std::decay_t<TArgs>...> args;

            template <typename Func, typename... Args>
            ThreadData(Func&& f, Args&&... a)
                : Run(&ThreadData::ThreadProc),
                  func(std::forward<Func>(f)),
                  args(std::forward<Args>(a)...)
            {
            }

            static void ThreadProc(void* self)
            {
                ThreadData* data = static_cast<ThreadData*>(self);
                std::apply(data->func, data->args);
            }
        };

        void StartThread(const char* threadName, void* data);

        Impl* impl;

      public:
        template <typename TFunc, typename... TArgs>
        Thread(TFunc&& func, TArgs&&... args)
        {
            using ThreadDataType = ThreadData<std::decay_t<TFunc>, std::decay_t<TArgs>...>;

            ThreadDataType* data = BlxMem::New<ThreadDataType>(
                BLXMEMORY_TAG_APPLICATION,
                std::forward<TFunc>(func),
                std::forward<TArgs>(args)...);

            StartThread(BLX_EMPTY_STRING, (void*)data);
        }

        template <typename TFunc, typename... TArgs>
        Thread(const char* threadName, TFunc&& func, TArgs&&... args)
        {
            using ThreadDataType = ThreadData<std::decay_t<TFunc>, std::decay_t<TArgs>...>;

            ThreadDataType* data = BlxMem::New<ThreadDataType>(
                BLXMEMORY_TAG_APPLICATION,
                std::forward<TFunc>(func),
                std::forward<TArgs>(args)...);

            StartThread(threadName, (void*)data);
        }

        void Join();

        static void Sleep(uint32 milliseconds);

        ~Thread();
    };
} // namespace BlxStl