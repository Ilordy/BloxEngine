#include "blx_window.h"
#include <Windows.h>
#include <winuser.h>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/GL.h>

//TODO: Deprecated stuff, pls remove.

typedef struct {
    //TODO: Clean up this struct with correct names.
    HWND hWnd;
    WNDCLASS wc;
    HGLRC context;
    HDC hdc;
    blxBool shouldClose;
}blxWin32Window;

LRESULT CALLBACK Wndproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    //TODO: Null check this and return if null. Does not get set before WM_CREATE.
    //blxWin32Window* win = (blxWin32Window*)GetProp(hWnd, "BLXWindow");
    switch (uMsg)
    {
    case WM_CLOSE:
        blxWin32Window* win = (blxWin32Window*)GetProp(hWnd, "BLXWindow");
        win->shouldClose = BLX_TRUE;
        DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        printf("post quit msg\n");

        return 0;
    case WM_KEYDOWN:

        break;
    case WM_CREATE:
        printf("Window requested to be created");
        break;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

blxWindow* blxCreateWindow(unsigned int width, unsigned int height)
{

    //Do platform stuff here later.
    blxWin32Window* window = (blxWin32Window*)malloc(sizeof(blxWin32Window));
    HINSTANCE hInstance = GetModuleHandle(NULL);
    const char* className = "Blox Window";
    WNDCLASS wc = { 0 };
    wc.lpszClassName = className;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpfnWndProc = Wndproc;

    RegisterClass(&wc);
    DWORD style = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_EX_APPWINDOW;

    HWND hWnd = CreateWindowEx(0, className, "Application", style, CW_USEDEFAULT,
        CW_USEDEFAULT, width, height, NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, SW_SHOW);

    window->hWnd = hWnd;
    window->shouldClose = BLX_FALSE;
    window->wc = wc;

    //TODO: This part below should go into the graphics class.
    //-----OpenGL part------
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
    window->hdc = dc;
    int pf = ChoosePixelFormat(dc, &pfd);
    SetPixelFormat(dc, pf, &pfd);
    HGLRC rc = wglCreateContext(dc);
    wglMakeCurrent(dc, rc);


    // TODO: remove from here
    if (glewInit() == GLEW_OK) {
        printf("GLEW INIT!!");
    }
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB =
        (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    //End
    HGLRC context = wglCreateContextAttribsARB(dc, NULL, attribs);
    wglMakeCurrent(dc, context);
    wglDeleteContext(rc);
    GLint versionMajor;
    glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
    GLint versionMinor;
    glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
    printf("OpenGL Version: %d.%d", versionMajor, versionMinor);
    window->context = context;
    SetProp(hWnd, "BLXWindow", window);
    glEnable(GL_DEPTH_TEST);
    return (blxWindow*)window;
}

blxBool blxWindowShouldClose(blxWindow* window)
{
    blxWin32Window* win32Win = (blxWin32Window*)window;
    MSG msg;
    while (PeekMessage(&msg, win32Win->hWnd, 0, 0, PM_REMOVE)) {

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    //TODO: abstract this away probably into a renderer.
    // glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    return win32Win->shouldClose;
}

void blxSwapBuffers(blxWindow* window)
{
    blxWin32Window* win32Win = (blxWin32Window*)window;
    SwapBuffers(win32Win->hdc);
}

void blxWindowClear()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void blxTerminateWindow(blxWindow* window)
{
    blxWin32Window* win32Win = (blxWin32Window*)window;
    ReleaseDC(win32Win->hWnd, win32Win->hdc);
    wglDeleteContext(win32Win->context);
    //TODO: Verify if using window's Atoms are better instead of specificy a string for unregistering a class
    //RegisterClass returns an Atom
    UnregisterClass(win32Win->wc.lpszClassName, GetModuleHandle(NULL));
    free(win32Win);
}


