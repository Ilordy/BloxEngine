#pragma once
#include "rendering/blx_mesh.h"
#include "core/blx_transform.h"
#include "rendering/blx_shader.h"

typedef struct {
    Transform transform;
    blxMesh mesh;
}blxModel;

BLXAPI void blxInitModel(blxModel* model);