#pragma once
#include "core/blx_defines.h"
#include "rendering/blx_shader.h"

#define BLX_MAX_MATERIALS 32

#define BLX_AUTO_UNLOAD_MAT __attribute__((cleanup(blxUnloadMaterial))) 

typedef struct {
    void* _internalData;
    unsigned int id;
    Shader shader;
}blxMaterial;

BLXAPI blxBool blxLoadMaterial(const char* path, blxMaterial** outMat);
BLXAPI void blxUnloadMaterial(blxMaterial** mat);

void _blxInitMaterialSystem();