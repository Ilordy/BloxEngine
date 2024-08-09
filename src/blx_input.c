#include "blx_input.h"
#include "blx_memory.h"
#include "blx_logger.h"

typedef struct {
    blxBool keys[256];
} keyboardState;

typedef struct {
    short x;
    short y;
    blxBool buttons[_BLX_BUTTON_MAX_BUTTONS];
} mouseState;

typedef struct {
    keyboardState keyboardCurrentState;
    keyboardState keyboardPrevState;
    mouseState mouseCurrentState;
    mouseState mousePrevState;
} inputState;

static blxBool initialized = BLX_FALSE;
static inputState state;


void _blxInitInputSystem()
{
    blxZeroMemory(&state, sizeof(inputState));
    initialized = BLX_TRUE;
    BLXINFO("Input system initialized.");
}

void _blxShutDownInputSystem()
{
    initialized = false;
}

void _blxUpdateInput(double deltaTime)
{
    if (!initialized) {
        return;
    }

    blxMemCpy(&state.keyboardPrevState, &state.keyboardCurrentState, sizeof(keyboardState));
    blxMemCpy(&state.mousePrevState, &state.mouseCurrentState, sizeof(mouseState));
}

void _blxInputProcessKey(blxKeyBoardInputs key, blxBool pressed)
{
    if (state.keyboardCurrentState.keys[key] != pressed) {
        state.keyboardCurrentState.keys[key] = pressed;
        // TODO: some events here for changing state
    }
}

void _blxInputProcessMouseButton(blxMouseButtonInputs button, blxBool pressed)
{
    if (state.mouseCurrentState.buttons[button] != pressed) {
        state.mouseCurrentState.buttons[button] = pressed;
        // TODO: some events here for changing state
    }
}

void _blxInputProcessMouseMove(short x, short y)
{
    if (state.mouseCurrentState.x != x || state.mouseCurrentState.y != y) {
        state.mouseCurrentState.x = x;
        state.mouseCurrentState.y = y;
        // TODO: some events here for changing state
    }
}

void _blxInputProcessMouseWheel(char delta)
{
    // TODO: Fire event for mouse delta.
}


blxBool blxInputIsKeyDown(blxKeyBoardInputs key)
{
    if (!initialized) {
        return BLX_FALSE;
    }

    return state.keyboardCurrentState.keys[key] == BLX_TRUE;
}

blxBool blxInputIsKeyUp(blxKeyBoardInputs key)
{
    if (!initialized) {
        return BLX_FALSE;
    }

    return state.keyboardCurrentState.keys[key] == BLX_FALSE;
}

blxBool blxIsMouseButtonDown(blxMouseButtonInputs button)
{
    if (!initialized) {
        return BLX_FALSE;
    }

    return state.mouseCurrentState.buttons[button] == BLX_FALSE;
}

blxBool blxIsMouseButtonUp(blxMouseButtonInputs button)
{
    if (!initialized) {
        return BLX_FALSE;
    }

    return state.mouseCurrentState.buttons[button] == BLX_FALSE;
}

void blxInputGetMousePos(int* x, int* y)
{
    if (!initialized) {
        *x = 0;
        *y = 0;
        return;
    }
    *x = state.mouseCurrentState.x;
    *y = state.mouseCurrentState.y;
}
























