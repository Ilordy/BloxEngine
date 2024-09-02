#pragma once
#include "core/blx_defines.h"
#include "core/blx_transform.h"
#include "utils/blx_vlist.h"
#include "rendering/blx_shader.h"
#include "cglm/struct/vec3.h"

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