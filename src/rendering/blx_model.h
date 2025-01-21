#pragma once
#include "rendering/blx_mesh.h"
#include "core/blx_transform.h"
#include "rendering/blx_shader.h"
#include "rendering/blx_material.h"
#include "utils/blx_utilityTypes.h"

//TODO: Make a bunch of typedefs in a generic header for for these list types.
//TODO: OR Just use a bunch of xml comments to describe their types.
typedef blxMesh* list_blxMesh;
typedef blxMaterial** list_blxMaterialPtrs;

typedef struct {
    Transform transform;
    list_blxMaterialPtrs materials;
    //TODO: this should most likely be a list of pointers to meshes...
    list_blxMesh meshes;
     /// @brief material and mesh array count (they share the same count).
    unsigned int mCount;
}blxModel;


BLXAPI void blxInitModel(blxModel* model);