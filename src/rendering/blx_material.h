#pragma once
#include "core/blx_defines.h"
#include "rendering/blx_renderingTypes.h"

//TODO: Max materials must be refactored.
#define BLX_MAX_MATERIALS 32

#define BLX_AUTO_UNLOAD_MAT __attribute__((cleanup(blxUnloadMaterial))) 

BLXAPI blxMaterial* blxMaterial_CreateDefault();

BLXAPI blxBool blxLoadMaterial(const char* path, blxMaterial** outMat);
BLXAPI void blxUnloadMaterial(blxMaterial** mat);

void _blxMaterialSetValues(blxMaterial* mat);
void _blxInitMaterialSystem();