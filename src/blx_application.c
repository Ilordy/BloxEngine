#include "blx_gameType.h"
#include "blx_application.h"
#include "blx_logger.h"
#include "internal/platform/platform.h"
#include "blx_memory.h"
#include "blx_input.h"

typedef struct {
    blxBool isRunning;
    blxBool isSuspended;
    platformState* platform;
    short width, height;
    double lastTime;
    GraphicsAPI currentGraphicAPI;
    blxGameInstance* gameInstance;
}appState;

static blxBool initialized = BLX_FALSE;
static appState app;

blxBool blxCreateApplication(blxGameInstance* gameInstance)
{
    if (initialized) {
        BLXERROR("Application already initialized");
        return BLX_FALSE;
    }

    app.gameInstance = gameInstance;
    blxInitalizeLogging();
    _blxInitInputSystem();

    app.isRunning = BLX_TRUE;
    //Only true when window is not focused.
    app.isSuspended = BLX_FALSE;


    if (!PlatformInit(&app.platform,
        gameInstance->config.name, gameInstance->config.width,
        gameInstance->config.height, gameInstance->config.graphicAPI)) {
        return BLX_FALSE;
    }

    if (!app.gameInstance->Init(app.gameInstance)) {
        BLXERROR("Game failed to initialize!");
        return BLX_FALSE;
    }

    //Causing issues, we must create a rendering context before as well.
    //blxInitRenderer(config->graphicAPI);
    initialized = BLX_TRUE;

    return BLX_TRUE;
}

blxBool blxRunApplication()
{
    //Game Loop
    while (app.isRunning)
    {
        if (!PlatformPumpMessages(&app.platform)) {
            app.isRunning = BLX_FALSE;
        }

        if (!app.isSuspended) {
            if (!app.gameInstance->Update(app.gameInstance, 0)) {
                BLXERROR("Game Update failed.");
                app.isRunning = BLX_FALSE;
                break;
            }
        }

        //todo: delta time
        _blxUpdateInput(0);
    }
    app.isRunning = BLX_FALSE;
    PlatformShutDown(&app.platform);
    _blxShutDownInputSystem();
    return BLX_TRUE;
}
