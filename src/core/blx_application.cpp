#include "blx_application.h"
BLXEXTERN_C_START
#include "blx_gameType.h"
#include "blx_input.h"
#include "blx_logger.h"
#include "blx_memory.h"
#include "internal/physicsWorld.h"
#include "internal/platform/platform.h"
#include "utils/blx_clock.h"

BLXEXTERN_C_END

typedef struct
{
    blxBool isRunning;
    blxBool isSuspended;
    platformState platform;
    short width, height;
    double lastTime;
    blxClock clock;
    blxGameInstance* gameInstance;
    // these 2 to renderer.
    GraphicsAPI currentGraphicAPI;
} appState;

static blxBool initialized = BLX_FALSE;
static appState app;

// TODO: Move this to a scene manager when we support multiple scenes.
static BlxCore::Scene mainScene;

BlxCore::Scene* blxGetMainScene()
{
    return &mainScene;
}

// TODO: This should be moved to a config file.
const double fixedTimeStep = 1.0f / 60; // 60 fps
double physicsAccumulator = 0.0;

blxBool blxCreateApplication(blxGameInstance* gameInstance)
{
    if (initialized)
    {
        BLXERROR("Application already initialized");
        return BLX_FALSE;
    }

    app.gameInstance = gameInstance;
    blxInitalizeLogging();
    _blxInitInputSystem();

    app.isRunning = BLX_TRUE;
    // Only true when window is not focused.
    app.isSuspended = BLX_FALSE;

    if (!blxPlatform_Init(&app.platform, gameInstance->config.name, gameInstance->config.width, gameInstance->config.height, gameInstance->config.graphicAPI))
    {
        BLXERROR("Platform failed to initialize!");
        return BLX_FALSE;
    }

    blxInitRenderer(app.currentGraphicAPI);
    InitializePhysicsWorld();

    if (!app.gameInstance->Init(app.gameInstance))
    {
        BLXERROR("Game failed to initialize!");
        return BLX_FALSE;
    }

    // Causing issues, we must create a rendering context before as well.
    // blxInitRenderer(config->graphicAPI);
    initialized = BLX_TRUE;

    return BLX_TRUE;
}

blxBool blxRunApplication()
{
    blxStartClock(&app.clock);
    blxUpdateClock(&app.clock);
    app.lastTime = app.clock.elaspedTime;
    double runningTime = 0;
    unsigned char frameCount = 0;
    double targetFrameSeconds = 1.0f / 60; // 60 is desired frame rate. (60 fps)

    // Game Loop
    while (app.isRunning)
    {
        if (!blxPlatform_PumpMessages(&app.platform))
        {
            app.isRunning = BLX_FALSE;
        }

        if (!app.isSuspended)
        {
            blxUpdateClock(&app.clock);
            double currentTime = app.clock.elaspedTime;
            double delta = currentTime - app.lastTime;
            double frameStartTime = blxPlatform_GetTime();

            if (!app.gameInstance->Update(delta))
            {
                BLXERROR("Game Update failed!");
                app.isRunning = BLX_FALSE;
                break;
            }

            if (!app.gameInstance->Render())
            {
                BLXERROR("Game Render failed!");
                app.isRunning = BLX_FALSE;
                break;
            }

            mainScene.Update(delta);

            physicsAccumulator += delta;
            while (physicsAccumulator >= fixedTimeStep)
            {
                // Update the physics world with the fixed delta time.
                UpdatePhysicsWorld(fixedTimeStep);
                physicsAccumulator -= fixedTimeStep;
            }

            blxDraw();
            blxPlatform_SwapBuffers();

            double frameEndTime = blxPlatform_GetTime();
            double frameElapsedTime = frameEndTime - frameStartTime;
            runningTime += frameElapsedTime;
            double remainingSeconds = targetFrameSeconds - frameElapsedTime;

            if (remainingSeconds > 0)
            {
                u64 remainingMs = remainingSeconds * 1000;

                // TODO: Add functionality for limiting frames and setting a desired frame rate.
                blxBool limitFrames = BLX_FALSE;
                if (remainingMs > 0 && limitFrames)
                {
                    blxPlatform_Sleep(remainingMs - 1);
                }

                frameCount++;
            }

            _blxUpdateInput(delta);
            app.lastTime = currentTime;
        }
    }
    app.isRunning = BLX_FALSE;
    blxPlatform_ShutDown(&app.platform);
    _blxShutDownInputSystem();
    return BLX_TRUE;
}
