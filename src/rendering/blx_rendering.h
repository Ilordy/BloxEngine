#pragma once
#include "core/blx_defines.h"
#include "rendering/blx_camera.h"
#include "rendering/blx_mesh.h"
#include "rendering/blx_model.h"
#include "rendering/lighting/blx_lightingTypes.h"

typedef enum {
    OPENGL
}GraphicsAPI;

typedef struct
{
    Camera* cam;
    blxModel models[10];
    blxModel ui[10];
    unsigned int modelCount;
    unsigned int uiCount;
    blxDirectionalLight directionalLight;
    blxPointLight pointLights[4];
}blxRenderPacket;

typedef enum {
    BLX_SHADING_SOLID,
    BLX_SHADING_WIREFRAME
}blxShadingMode;

void blxDrawUI(blxModel* model);

BLXAPI void blxDrawModel(blxModel* model);
BLXAPI void blxInitRenderer(GraphicsAPI graphicsToUse);
BLXAPI void blxInitMesh(blxMesh* mesh);
BLXAPI void blxSetShadingMode(blxShadingMode mode);

// TODO: THIS is temp for now for in the future of support more than 1 camera.
BLXAPI void blxAddCameraToRender(Camera* cam);

BLXAPI void blxDraw();
BLXAPI void blxUpdateMesh(blxMesh* mesh);
BLXAPI void blxImportMesh(const char* filePath, blxMesh* outMesh);




