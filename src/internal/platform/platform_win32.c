#include "platform.h"

#ifdef BLXWIN32
#include "blx_input.h"
#include <Windows.h>
#include <windowsx.h>
#include <time.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/GL.h>

typedef struct
{
    HINSTANCE hInstance;
    HWND hwnd;
    HDC hdc;
} internalState;

static internalState* state;
static double clockFrequency;
static LARGE_INTEGER startTime;

LRESULT CALLBACK WindowMsgProcess(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

static blxBool CreateRenderingContext(GraphicsAPI graphicsAPI, HWND hWnd)
{
    switch (graphicsAPI)
    {
        case OPENGL:
            //Setting GL version to 4.3 using core profile.
            GLint attribs[] = { WGL_CONTEXT_MAJOR_VERSION_ARB, 4,  WGL_CONTEXT_MINOR_VERSION_ARB, 3,
             WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB, 0 };
            PIXELFORMATDESCRIPTOR pfd =
            {
                sizeof(PIXELFORMATDESCRIPTOR),
                1,
                PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, // Flags
                PFD_TYPE_RGBA,                                              // The kind of framebuffer. RGBA or palette.
                32,                                                         // Colordepth of the framebuffer.
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                0,
                24, // Number of bits for the depthbuffer
                8,  // Number of bits for the stencilbuffer
                0,  // Number of Aux buffers in the framebuffer.
                PFD_MAIN_PLANE,
                0,
                0,
                0,
                0 };
            HDC dc = GetDC(hWnd);
            state->hdc = dc;
            int pf = ChoosePixelFormat(dc, &pfd);
            SetPixelFormat(dc, pf, &pfd);
            HGLRC rc = wglCreateContext(dc);
            wglMakeCurrent(dc, rc);

            if (glewInit() != GLEW_OK) {
                MessageBoxA(0, "Failed to initialize OpenGL!", "Error", MB_ICONEXCLAMATION | MB_OK);
                return BLX_FALSE;
            }
            
            PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB =
                (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
            HGLRC context = wglCreateContextAttribsARB(dc, NULL, attribs);
            wglMakeCurrent(dc, context);
            wglDeleteContext(rc);
            break;
    }

    return BLX_TRUE;
}

blxBool PlatformInit(platformState* platform, const char* appName, unsigned short width, unsigned short height, GraphicsAPI graphicsAPI)
{
    platform->internalState = malloc(sizeof(internalState));
    state = (internalState*)platform->internalState;

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

    if (!CreateRenderingContext(graphicsAPI, hWnd)) {
        return BLX_FALSE;
    }

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
        //TODO: Not sure if release DC is opengl specific must be researched.
        ReleaseDC(state->hwnd, state->hdc);
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
            DestroyWindow(hWnd);
            // TODO: EVENT FOR POST QUITNG!
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
            break;

        case WM_KEYDOWN: //Regular key
        case WM_SYSKEYDOWN: //Key while holding down ALT
        case WM_KEYUP:
        case WM_SYSKEYUP: {
            blxBool pressed = (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN);
            blxKeyBoardInputs key = (short)wParam;
            //Simple for windows however will need to be translated for other platforms.
            _blxInputProcessKey(key, pressed);
        }break;
        case WM_MOUSEMOVE: {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            _blxInputProcessMouseMove(x, y);

        }break;
        case WM_MOUSEWHEEL: {
            int wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            //something has happened if not 0
            if (wheelDelta != 0) {
                wheelDelta = (wheelDelta < 0) ? -1 : 1;
                _blxInputProcessMouseWheel(wheelDelta);
            }
        }break;
        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
        case WM_RBUTTONUP: {
            blxBool pressed = uMsg == WM_LBUTTONDOWN || uMsg == WM_RBUTTONDOWN || uMsg == WM_MBUTTONDOWN;
            //setting to default
            blxMouseButtonInputs button = _BLX_BUTTON_MAX_BUTTONS;
            switch (uMsg) {
                case WM_LBUTTONDOWN:
                case WM_LBUTTONUP:
                    button = BLX_LMB;
                    break;
                case WM_MBUTTONDOWN:
                case WM_MBUTTONUP:
                    button = BLX_MMB;
                    break;
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:
                    button = BLX_RMB;
                    break;
            }

            if (button != _BLX_BUTTON_MAX_BUTTONS) {
                _blxInputProcessMouseButton(button, pressed);
            }
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

void PlatformSleep(uint64 ms)
{
    Sleep(ms);
}

blxBool PlatformOpenFilePanel(const char* title, const char* defDirectory, const char* extension, char* buffer)
{
    OPENFILENAME ofn; // Common dialog box structure
    char strFilter[100];
    snprintf(strFilter, sizeof(strFilter), "(*%s)%c*%s%c%c", extension, '\0', extension, '\0');

    // Initialize OPENFILENAME
    PlatformMemSetZero(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = state->hwnd;
    ofn.lpstrFile = buffer;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = strFilter;
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = defDirectory;
    ofn.lpstrTitle = title;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        return BLX_TRUE;
    }

    return BLX_FALSE;
}

void PlatformSwapBuffers()
{
    //TODO: Figure out if this is opengl specific.
    SwapBuffers(state->hdc);
}

#endif