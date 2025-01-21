#pragma once
#include "core/blx_defines.h"

//TODO: Some of these things should be internal data only!

typedef unsigned int Shader;

//TODO: A name must be given! for all assets!
typedef struct {
    void* _internalData;
    unsigned int id;
    Shader shader;
}blxMaterial;

typedef enum {
    OPENGL
}GraphicsAPI;

typedef struct {
    blxMaterial* material;
    void* internalData;
}_blxRenderBatch;