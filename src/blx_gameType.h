#pragma once

#include "blx_application.h"

typedef struct blxGameInstance {
    blxApplicationConfig config;
    /// @brief Initialize function for game
    blxBool(*Init)(struct blxGameInstance* game);
    /// Called once per frame
    blxBool(*Update)(struct blxGameInstance* game, float deltaTime);
    void* state;
}blxGameInstance;