#pragma once
#include "core/blx_defines.h"
#include "rendering/blx_camera.h"
#include "rendering/blx_mesh.h"
#include "rendering/blx_model.h"
#include "rendering/lighting/blx_lightingTypes.h"
#include "rendering/blx_material.h"

//TODO: Renderer backend/internal file for data types/ functions the user should not be using Such as the MaterialGroup...
//TODO: Change to BatchGroup or similar name.
// TODO: Only blxRenderableGeometry* is needed not really verts or indices.
typedef struct MaterialGroup {
    blxMaterial* material;
    //TODO: These meshes should be registered by a resource system.
    //TODO: This should be a hashset instead of a dyamic list.
    blxMesh* vlist_meshes;
    void* renderData;
    vList_blxVertex* vertices;
    vList_indices* indices;
    blxRenderableGeometry* geometries;
}MaterialGroup;

typedef struct
{
    Camera* cam;
    blxModel models[10];
    blxModel ui[10];
    unsigned int modelCount;
    unsigned int uiCount;
    blxDirectionalLight directionalLight;
    blxPointLight pointLights[4];
    struct MaterialGroup* vlist_materialGroups;
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
BLXAPI void blxMeshUseMaterial(blxMesh* mesh, blxMaterial* mat);

void _blxRendererRegisterMaterial(blxMaterial* material);

