#include "core/blx_defines.h"

typedef enum {
    BLX_LMB,
    BLX_RMB,
    BLX_MMB,
    _BLX_BUTTON_MAX_BUTTONS
}blxMouseButtonInputs;

#define _DEFINE_KEY(name, code) BLX_KEY_##name = code

typedef enum {
    _DEFINE_KEY(BACKSPACE, 0x08),
    _DEFINE_KEY(ENTER, 0x0D),
    _DEFINE_KEY(TAB, 0x09),
    _DEFINE_KEY(SHIFT, 0x10),
    _DEFINE_KEY(CONTROL, 0x11),

    _DEFINE_KEY(PAUSE, 0x13),
    _DEFINE_KEY(CAPITAL, 0x14),

    _DEFINE_KEY(ESCAPE, 0x1B),

    _DEFINE_KEY(CONVERT, 0x1C),
    _DEFINE_KEY(NONCONVERT, 0x1D),
    _DEFINE_KEY(ACCEPT, 0x1E),
    _DEFINE_KEY(MODECHANGE, 0x1F),

    _DEFINE_KEY(SPACE, 0x20),
    _DEFINE_KEY(PRIOR, 0x21),
    _DEFINE_KEY(NEXT, 0x22),
    _DEFINE_KEY(END, 0x23),
    _DEFINE_KEY(HOME, 0x24),
    _DEFINE_KEY(LEFT, 0x25),
    _DEFINE_KEY(UP, 0x26),
    _DEFINE_KEY(RIGHT, 0x27),
    _DEFINE_KEY(DOWN, 0x28),
    _DEFINE_KEY(SELECT, 0x29),
    _DEFINE_KEY(PRINT, 0x2A),
    _DEFINE_KEY(EXECUTE, 0x2B),
    _DEFINE_KEY(SNAPSHOT, 0x2C),
    _DEFINE_KEY(INSERT, 0x2D),
    _DEFINE_KEY(DELETE, 0x2E),
    _DEFINE_KEY(HELP, 0x2F),

    _DEFINE_KEY(0, 0x30),
    _DEFINE_KEY(1, 0x31),
    _DEFINE_KEY(2, 0x32),

    _DEFINE_KEY(A, 0x41),
    _DEFINE_KEY(B, 0x42),
    _DEFINE_KEY(C, 0x43),
    _DEFINE_KEY(D, 0x44),
    _DEFINE_KEY(E, 0x45),
    _DEFINE_KEY(F, 0x46),
    _DEFINE_KEY(G, 0x47),
    _DEFINE_KEY(H, 0x48),
    _DEFINE_KEY(I, 0x49),
    _DEFINE_KEY(J, 0x4A),
    _DEFINE_KEY(K, 0x4B),
    _DEFINE_KEY(L, 0x4C),
    _DEFINE_KEY(M, 0x4D),
    _DEFINE_KEY(N, 0x4E),
    _DEFINE_KEY(O, 0x4F),
    _DEFINE_KEY(P, 0x50),
    _DEFINE_KEY(Q, 0x51),
    _DEFINE_KEY(R, 0x52),
    _DEFINE_KEY(S, 0x53),
    _DEFINE_KEY(T, 0x54),
    _DEFINE_KEY(U, 0x55),
    _DEFINE_KEY(V, 0x56),
    _DEFINE_KEY(W, 0x57),
    _DEFINE_KEY(X, 0x58),
    _DEFINE_KEY(Y, 0x59),
    _DEFINE_KEY(Z, 0x5A),

    _DEFINE_KEY(LWIN, 0x5B),
    _DEFINE_KEY(RWIN, 0x5C),
    _DEFINE_KEY(APPS, 0x5D),

    _DEFINE_KEY(SLEEP, 0x5F),

    _DEFINE_KEY(NUMPAD0, 0x60),
    _DEFINE_KEY(NUMPAD1, 0x61),
    _DEFINE_KEY(NUMPAD2, 0x62),
    _DEFINE_KEY(NUMPAD3, 0x63),
    _DEFINE_KEY(NUMPAD4, 0x64),
    _DEFINE_KEY(NUMPAD5, 0x65),
    _DEFINE_KEY(NUMPAD6, 0x66),
    _DEFINE_KEY(NUMPAD7, 0x67),
    _DEFINE_KEY(NUMPAD8, 0x68),
    _DEFINE_KEY(NUMPAD9, 0x69),
    _DEFINE_KEY(MULTIPLY, 0x6A),
    _DEFINE_KEY(ADD, 0x6B),
    _DEFINE_KEY(SEPARATOR, 0x6C),
    _DEFINE_KEY(SUBTRACT, 0x6D),
    _DEFINE_KEY(DECIMAL, 0x6E),
    _DEFINE_KEY(DIVIDE, 0x6F),
    _DEFINE_KEY(F1, 0x70),
    _DEFINE_KEY(F2, 0x71),
    _DEFINE_KEY(F3, 0x72),
    _DEFINE_KEY(F4, 0x73),
    _DEFINE_KEY(F5, 0x74),
    _DEFINE_KEY(F6, 0x75),
    _DEFINE_KEY(F7, 0x76),
    _DEFINE_KEY(F8, 0x77),
    _DEFINE_KEY(F9, 0x78),
    _DEFINE_KEY(F10, 0x79),
    _DEFINE_KEY(F11, 0x7A),
    _DEFINE_KEY(F12, 0x7B),
    _DEFINE_KEY(F13, 0x7C),
    _DEFINE_KEY(F14, 0x7D),
    _DEFINE_KEY(F15, 0x7E),
    _DEFINE_KEY(F16, 0x7F),
    _DEFINE_KEY(F17, 0x80),
    _DEFINE_KEY(F18, 0x81),
    _DEFINE_KEY(F19, 0x82),
    _DEFINE_KEY(F20, 0x83),
    _DEFINE_KEY(F21, 0x84),
    _DEFINE_KEY(F22, 0x85),
    _DEFINE_KEY(F23, 0x86),
    _DEFINE_KEY(F24, 0x87),

    _DEFINE_KEY(NUMLOCK, 0x90),
    _DEFINE_KEY(SCROLL, 0x91),

    _DEFINE_KEY(NUMPAD_EQUAL, 0x92),

    _DEFINE_KEY(LSHIFT, 0xA0),
    _DEFINE_KEY(RSHIFT, 0xA1),
    _DEFINE_KEY(LCONTROL, 0xA2),
    _DEFINE_KEY(RCONTROL, 0xA3),
    _DEFINE_KEY(LMENU, 0xA4),
    _DEFINE_KEY(RMENU, 0xA5),

    _DEFINE_KEY(SEMICOLON, 0xBA),
    _DEFINE_KEY(PLUS, 0xBB),
    _DEFINE_KEY(COMMA, 0xBC),
    _DEFINE_KEY(MINUS, 0xBD),
    _DEFINE_KEY(PERIOD, 0xBE),
    _DEFINE_KEY(SLASH, 0xBF),
    _DEFINE_KEY(GRAVE, 0xC0),

    _BLX_KEYS_MAX_KEYS
} blxKeyBoardInputs;


BLXAPI blxBool blxInputGetKey(blxKeyBoardInputs key);
BLXAPI blxBool blxInputGetKeyDown(blxKeyBoardInputs button);
BLXAPI blxBool blxInputGetKeyUp(blxKeyBoardInputs key);

BLXAPI blxBool blxInputWasKeyDown(blxKeyBoardInputs key);
BLXAPI blxBool blxInputWasKeyUp(blxKeyBoardInputs key);

BLXAPI blxBool blxInputGetMouseButton(blxMouseButtonInputs button);
BLXAPI blxBool blxGetMouseButtonDown(blxMouseButtonInputs button);
BLXAPI blxBool blxGetMouseButtonUp(blxMouseButtonInputs button);
BLXAPI blxBool blxWasMouseButtonDown(blxMouseButtonInputs button);
BLXAPI blxBool blxWasMouseButtonUp(blxMouseButtonInputs button);

BLXAPI void blxInputGetMousePos(int* x, int* y);
BLXAPI void blxInputGetPrevMousePos(int* x, int* y);

void _blxInitInputSystem();
void _blxShutDownInputSystem();
void _blxUpdateInput(double deltaTime);
void _blxInputProcessKey(blxKeyBoardInputs key, blxBool pressed);
void _blxInputProcessMouseButton(blxMouseButtonInputs button, blxBool pressed);
void _blxInputProcessMouseMove(short x, short y);
void _blxInputProcessMouseWheel(char delta);