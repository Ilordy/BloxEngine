#pragma once
#include "core/blx_defines.h"
#include "rendering/blx_rendering.h"

struct blxGameInstance;

// Main application instance.

typedef struct blxApplicationConfig {
    unsigned short width, height;
    char* name;
    GraphicsAPI graphicAPI;
}blxApplicationConfig;

/// @brief Should be called by blx_entry.h Only.
BLXAPI blxBool blxCreateApplication(struct blxGameInstance* gameInstance);

/// @brief Should be called by blx_entry.h Only.
BLXAPI blxBool blxRunApplication();