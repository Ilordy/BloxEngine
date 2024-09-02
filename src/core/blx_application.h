#pragma once
#include "core/blx_defines.h"
#include "rendering/blx_rendering.h"

struct blxGameInstance;

typedef struct blxApplicationConfig {
    unsigned short width, height;
    char* name;
    GraphicsAPI graphicAPI;
}blxApplicationConfig;

BLXAPI blxBool blxCreateApplication(struct blxGameInstance* gameInstance);
BLXAPI blxBool blxRunApplication();