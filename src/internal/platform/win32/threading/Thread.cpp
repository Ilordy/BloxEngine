#include "threading/Thread.h"



#ifdef BLXWIN32
#include <windows.h>
#include <handleapi.h>
#include <processthreadsapi.h>

#include "utils/blx_assertions.h"

namespace BlxStl
{
    struct Thread::Impl
    {
        HANDLE hThread;
        DWORD threadId;
    };

    DWORD WINAPI ThreadProc(LPVOID threadData)
    {
        // The first member of ThreadData is the function pointer,
        // so we can safely cast and dereference it to get the function to call.
        void (*Run)(void*) = *(void (**)(void*))threadData;
        Run((void*)threadData);

        // Clean up the thread data after the function has finished executing.
        BlxMem::DestructAndFree(threadData, BLXMEMORY_TAG_APPLICATION);
        return 0;
    }

    void Thread::StartThread(const char* threadName, void* data)
    {
        impl = BlxMem::New<Impl>(BLXMEMORY_TAG_APPLICATION);

        HANDLE hThread = CreateThread(
            NULL,            // default security attributes
            0,               // use default stack size
            ThreadProc,      // thread function name
            data,            // argument to thread function
            0,               // use default creation flags
            &impl->threadId); // returns the thread identifier

        if (hThread == NULL)
        {
            // Handle error
            // TODO: Add error handling for thread creation failure.
            BLXASSERT(BLX_FALSE);
        }
        else
        {
            impl->hThread = hThread;

            if (!blxStr_NullOrEmpty(threadName))
            {
                BLXASSERT_MSG((blxStr_Len(threadName) * sizeof(char)) < (64 * sizeof(char)), "Thread name is too long! Max length is 63 characters.");

                const int bufferSize = sizeof(WCHAR) * 64;
                WCHAR wideThreadName[bufferSize];

                MultiByteToWideChar(CP_UTF8, 0, threadName, -1, wideThreadName, bufferSize);
                SetThreadDescription(impl->hThread, wideThreadName);
            }
        }
    }

    void Thread::Join()
    {
        BLXASSERT_MSG(GetCurrentThreadId() != impl->threadId, 
        "Attempting to join the thread that is currently running! This will cause a deadlock.");

        WaitForSingleObject(impl->hThread, INFINITE);
    }

    void Thread::Sleep(uint32 milliseconds)
    {
        ::Sleep(milliseconds);
    }

    Thread::~Thread()
    {
        CloseHandle(impl->hThread);
        BlxMem::DestructAndFree(impl, BLXMEMORY_TAG_APPLICATION);
    }
} // namespace BlxStl

#endif
