#include "platform.h"

//TEMP FOR NOW.
#define BLXWIN32

//#ifdef BLXWIN32
#include <Windows.h>
#include <windowsx.h>
#include <time.h>
typedef struct
{
    HINSTANCE hInstance;
    HWND hwnd;
} internalState;

static double clockFrequency;
static LARGE_INTEGER startTime;

LRESULT CALLBACK WindowMsgProcess(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void CreateRenderingContext(GraphicsAPI graphicsAPI)
{

}

blxBool PlatformInit(platformState* platform, const char* appName, unsigned short width, unsigned short height, GraphicsAPI graphicsAPI)
{
    platform->internalState = malloc(sizeof(internalState));
    internalState* state = (internalState*)platform->internalState;

    state->hInstance = GetModuleHandleA(0);
    //Default icon, TODO: Support custom icons.
    HICON icon = LoadIcon(state->hInstance, IDI_APPLICATION);

    WNDCLASSA wc = { 0 };
    wc.style = CS_DBLCLKS;
    wc.hIcon = icon;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = state->hInstance;
    wc.hbrBackground = NULL;
    wc.lpfnWndProc = WindowMsgProcess;
    wc.lpszClassName = "blxWindowClass";

    if (!RegisterClassA(&wc)) {
        MessageBoxA(0, "Window reigstration failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return BLX_FALSE;
    }

    DWORD style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_THICKFRAME;
    DWORD exStyle = WS_EX_APPWINDOW;

    RECT borderRect = { 0,0,0,0 };
    AdjustWindowRectEx(&borderRect, style, 0, exStyle);

    width += borderRect.right - borderRect.left;
    height += borderRect.bottom - borderRect.top;

    HWND hWnd = CreateWindowExA(exStyle, "blxWindowClass", appName, style, CW_USEDEFAULT,
        CW_USEDEFAULT, width, height, NULL, NULL, state->hInstance, NULL);

    if (!hWnd) {
        MessageBoxA(NULL, "Window creation failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return BLX_FALSE;
    }
    else {
        state->hwnd = hWnd;
    }

    //Something about window not showing cmd if window should not accept input??
    ShowWindow(hWnd, SW_SHOW);
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    clockFrequency = 1.0 / (double)frequency.QuadPart;
    QueryPerformanceFrequency(&startTime);
    return BLX_TRUE;
}

void PlatformShutDown(platformState* platform)
{
    internalState* state = (internalState*)platform->internalState;

    if (state->hwnd) {
        DestroyWindow(state->hwnd);
        state->hwnd = 0;
    }
}

blxBool PlatformPumpMessages(platformState* platform)
{
    MSG msg;
    while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    return BLX_TRUE;
}

void PlatformWriteToConsole(const char* msg, uint8_t color)
{
    //Test this to just output certain text red.
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    // Error, warning, info, debug.
    static uint8_t levels[6] = { 4, 6, 2, 7 };
    SetConsoleTextAttribute(handle, levels[color] | FOREGROUND_INTENSITY);
    OutputDebugStringA(msg);
    double length = strlen(msg);
    LPDWORD numWritten = 0;
    WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE), msg, (DWORD)length, numWritten, 0);
}

void PlatformWriteToConsoleError(const char* msg)
{
    HANDLE handle = GetStdHandle(STD_ERROR_HANDLE);
    SetConsoleTextAttribute(handle, FOREGROUND_RED);
    OutputDebugStringA(msg);
    double length = strlen(msg);
    LPDWORD numWritten = 0;
    WriteConsoleA(GetStdHandle(STD_ERROR_HANDLE), msg, (DWORD)length, numWritten, 0);
}

double PlatformGetTime()
{
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);
    return (double)currentTime.QuadPart * clockFrequency;
}

LRESULT CALLBACK WindowMsgProcess(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //TODO: Null check this and return if null. Does not get set before WM_CREATE.
    //blxWin32Window* win = (blxWin32Window*)GetProp(hWnd, "BLXWindow");
    switch (uMsg)
    {
        case WM_ERASEBKGND:
            // To prevent flickering.
            return 1;
        case WM_CLOSE:
            //blxWin32Window* win = (blxWin32Window*)GetProp(hWnd, "BLXWindow");
            //win->shouldClose = BLX_TRUE;
            //DestroyWindow(hWnd);
            //For event system.
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_SIZE: {
            RECT r;
            GetClientRect(hWnd, &r);
            float width = r.right - r.left;
            float height = r.bottom - r.top;
            //Resize event.
        }break;
        case WM_CREATE:
            printf("Window requested to be created");
            break;

        case WM_KEYDOWN: //Regular key
        case WM_SYSKEYDOWN: //Key while holding down ALT
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            blxBool pressed = (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN);
            //Input processing goes here.
        }break;
        case WM_MOUSEMOVE: {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
           //Mouse move event.

        }break;
        case WM_MOUSEWHEEL: {
            int wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            //something has happened if not 0
            if (wheelDelta != 0) {
                wheelDelta = (wheelDelta < 0) ? -1 : 1;
            }
        }break;
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP: {
            blxBool pressed = uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN;

        }break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


void* PlatformAllocate(unsigned long long size)
{
    //TODO: Platform specific memory functionality.
    return malloc(size);
}

void* PlatformMemSetZero(void* block, unsigned long long size)
{
    return memset(block, 0, size);
}

void PlatformFreeMemory(void* block)
{
    free(block);
}

void* PlatformMemCpy(void* dest, const void* src, unsigned long long size)
{
    return memcpy(dest, src, size);
}

//#endif