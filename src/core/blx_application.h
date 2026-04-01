#pragma once
#include "Scene.h"
#include "blx_defines.h"
#include "core/blx_defines.h"
#include "rendering/blx_renderingTypes.h"

BLXEXTERN_C_START
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

BLXEXTERN_C_END

// TODO: This should be moved to a scene manager when we support multiple scenes.
// TODO: We should allow scene creation for the user to then register with the scene manager, but for now we will just have a single main scene.
BLXAPI BlxCore::Scene* blxGetMainScene();