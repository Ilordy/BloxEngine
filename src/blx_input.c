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


blxBool blxInputGetKey(blxKeyBoardInputs key)
{
    if (!initialized) {
        return BLX_FALSE;
    }

    return state.keyboardCurrentState.keys[key] == BLX_TRUE;
}

blxBool blxInputWasKeyDown(blxKeyBoardInputs key)
{
    if (!initialized) {
        return BLX_FALSE;
    }

    return state.keyboardPrevState.keys[key] == BLX_TRUE;
}

blxBool blxInputGetKeyUp(blxKeyBoardInputs key)
{
    if (!initialized) {
        return BLX_FALSE;
    }

    return state.keyboardPrevState.keys[key] == BLX_TRUE && state.keyboardCurrentState.keys[key] == BLX_FALSE;
}

blxBool blxInputWasKeyUp(blxKeyBoardInputs key)
{
    if (!initialized) {
        return BLX_FALSE;
    }

    return state.keyboardPrevState.keys[key] == BLX_FALSE;
}

blxBool blxGetMouseButtonDown(blxMouseButtonInputs button)
{
    if (!initialized) {
        return BLX_FALSE;
    }

    return state.mousePrevState.buttons[button] == BLX_FALSE && state.mouseCurrentState.buttons[button] == BLX_TRUE;
}

blxBool blxInputGetMouseButton(blxMouseButtonInputs button)
{
    if (!initialized) {
        return BLX_FALSE;
    }

    return state.mouseCurrentState.buttons[button] == BLX_TRUE;
}

blxBool blxWasMouseButtonDown(blxMouseButtonInputs button)
{
    if (!initialized) {
        return BLX_FALSE;
    }

    return state.mousePrevState.buttons[button] == BLX_TRUE;
}

blxBool blxGetMouseButtonUp(blxMouseButtonInputs button)
{
    if (!initialized) {
        return BLX_FALSE;
    }

    return state.mousePrevState.buttons[button] == BLX_TRUE && state.mouseCurrentState.buttons[button] == BLX_FALSE;
}

blxBool blxWasMouseButtonUp(blxMouseButtonInputs button)
{
    if (!initialized) {
        return BLX_FALSE;
    }

    return state.mousePrevState.buttons[button] == BLX_FALSE;
}

BLXAPI blxBool blxInputGetKeyDown(blxKeyBoardInputs key)
{
    if (!initialized) {
        return BLX_FALSE;
    }

    return state.keyboardPrevState.keys[key] == BLX_FALSE && state.keyboardCurrentState.keys[key] == BLX_TRUE;
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

void blxInputGetPrevMousePos(int* x, int* y)
{
    if (!initialized) {
        *x = 0;
        *y = 0;
        return;
    }

    *x = state.mousePrevState.x;
    *y = state.mousePrevState.y;
}
























