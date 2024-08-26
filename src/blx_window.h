#pragma once
#include "Defines.h"

//TODO: Deprecated stuff, pls remove.

typedef struct blxWindow blxWindow;

BLXAPI blxWindow* blxCreateWindow(unsigned int width, unsigned int height);
BLXAPI blxBool blxWindowShouldClose(blxWindow* window);
BLXAPI void blxTerminateWindow(blxWindow* window);
BLXAPI void blxSwapBuffers(blxWindow* window);
BLXAPI void blxWindowClear();