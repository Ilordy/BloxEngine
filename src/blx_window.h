#pragma once
#include "Defines.h"

typedef struct blxWindow blxWindow;

BLXAPI blxWindow* blxCreateWindow(unsigned int width, unsigned int height);
BLXAPI blxBool blxWindowShouldClose(blxWindow* window);
BLXAPI void blxTerminateWindow(blxWindow* window);