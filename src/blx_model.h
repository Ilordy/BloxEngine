#pragma once
#include "blx_mesh.h"
#include "Transform.h"
#include "Shader.h"

typedef struct {
    Transform transform;
    blxMesh mesh;
}blxModel;

BLXAPI void blxInitModel(blxModel* model);