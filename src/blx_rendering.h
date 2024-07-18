#pragma once
#include "Defines.h"
#include "Transform.h"
#include "utils/blx_vlist.h"
#include "Shader.h"
#include "cglm/struct/vec3.h"

typedef enum {
    OPENGL
}GraphicsAPI;

typedef struct {
    vec3s position;
    vec3s normal;
    vec2s uv;
}blxVertex;

typedef blxVertex vList_blxVertex;
typedef unsigned int vList_indices;

typedef struct {
    vList_blxVertex* vertices;
    vList_indices* indices;
    Shader shader;
    /// @brief Internal use only
    void* _meshData;
} blxMesh;

BLXAPI void blxInitRenderer(GraphicsAPI graphicsToUse);
BLXAPI void blxInitMesh(blxMesh* mesh);

//TODO: Make a blx_mesh.h instead..
BLXAPI void blxDraw(blxMesh* mesh);
BLXAPI void blxUpdateMesh(blxMesh* mesh);
BLXAPI void blxImportMesh(const char* filePath, blxMesh* outMesh);




