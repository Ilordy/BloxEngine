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
#define BLX_MAX_MATERIALS 100

#define BLX_AUTO_UNLOAD_MAT __attribute__((cleanup(blxUnloadMaterial))) 

BLXAPI blxMaterial* blxMaterial_CreateDefault();


/// @brief Sets a value for a given material.
/// @param mat The material to set the value for.
/// @param propName The name of the property to set.
/// @param propType The type of the property to set.
/// @param propValue The value of the property to set.
BLXAPI void blxMaterial_SetValue(blxMaterial* mat, const char* propName, blxMaterialPropertyType propType, void* propValue);

/// @brief Loads a material from a given file path.
/// @param path The path to the material file.
/// @param outMat A pointer to the loaded material.
/// @return True if the material was loaded successfully, false otherwise.
BLXAPI blxBool blxMaterial_Load(const char* path, blxMaterial** outMat);
BLXAPI void blxUnloadMaterial(blxMaterial** mat);

void _blxMaterialSetValues(blxMaterial* mat);
void _blxInitMaterialSystem();