#pragma once
#include "BLX/core/blx_defines.h"
#include "BLX/core/blx_gameType.h"
#include "BLX/core/blx_gameType.h"
#include "rendering/blx_camera.h"

typedef struct {
    float deltaTime;
    Camera mainCam;
    blxModel currentModel;
}gameState;

blxBool InitGame(blxGameInstance* gameInstance);

blxBool UpdateGame(float deltaTime);

blxBool Render();