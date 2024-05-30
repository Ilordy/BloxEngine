#pragma once
#include "Defines.h"
#include "Transform.h"
#include "cglm/cglm.h"
#include "utils/blx_vlist.h"
#include "Shader.h"

typedef enum {
    OPENGL
}GraphicsAPI;

typedef struct {
    vec3 position;
    vec3 normal;
    vec2 uv;
}blxVertex;

typedef blxVertex vList_blxVertex;
typedef unsigned int vList_indices;

typedef struct {
    vList_blxVertex* vertices;
    vList_indices* indices;
    Shader shader;
    /// @brief Internal use only
    void* _meshData;
}blxMesh;

BLXAPI void blxInitRenderer(GraphicsAPI graphicsToUse);
BLXAPI blxMesh* blxCreateMesh();
BLXAPI void blxDraw(blxMesh* mesh);
BLXAPI void blxUpdateMesh(blxMesh* mesh);




