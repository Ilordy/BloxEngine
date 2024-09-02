#pragma once

#include "blx_application.h"

typedef struct blxGameInstance {
    blxApplicationConfig config;
    /// @brief Initialize function for game
    blxBool(*Init)(struct blxGameInstance* game);
    /// Called once per frame
    blxBool(*Update)(float deltaTime);
     /// @brief Rendering goes here.
    blxBool(*Render)();
}blxGameInstance;