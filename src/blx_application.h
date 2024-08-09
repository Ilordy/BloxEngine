#pragma once
#include "Defines.h"
#include "blx_rendering.h"

struct blxGameInstance;

typedef struct blxApplicationConfig {
    unsigned short width, height;
    char* name;
    GraphicsAPI graphicAPI;
}blxApplicationConfig;

BLXAPI blxBool blxCreateApplication(struct blxGameInstance* gameInstance);
BLXAPI blxBool blxRunApplication();