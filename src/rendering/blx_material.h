#pragma once
#include "core/blx_defines.h"
#include "rendering/blx_renderingTypes.h"

typedef enum {
    BLX_MAT_PROP_INT,
    BLX_MAT_PROP_FLOAT,
    BLX_MAT_PROP_VEC3,
    BLX_MAT_PROP_VEC4
}blxMaterialPropertyType;



//TODO: Max materials must be refactored.
#define BLX_MAX_MATERIALS 32

#define BLX_AUTO_UNLOAD_MAT __attribute__((cleanup(blxUnloadMaterial))) 

BLXAPI blxMaterial* blxMaterial_CreateDefault();

//TODO: Test if this works with literals.
BLXAPI void blxMaterial_SetValue(blxMaterial* mat, const char* propName, blxMaterialPropertyType propType, void* propValue);

BLXAPI blxBool blxLoadMaterial(const char* path, blxMaterial** outMat);
BLXAPI void blxUnloadMaterial(blxMaterial** mat);

void _blxMaterialSetValues(blxMaterial* mat);
void _blxInitMaterialSystem();