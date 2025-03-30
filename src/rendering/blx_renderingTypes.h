#pragma once
#include "core/blx_defines.h"
#include "cglm/struct/vec3.h"
#include "core/blx_transform.h"
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
    /// @brief Internal use only
    void* _meshData;
} blxMesh;

// TODO: MAKE INITIALIZER FUNCTIONS FOR THIS BUT FIRST RESEARCH WHAT INLINING DOES AND USING IT WITH STATIC AS WELL!

typedef struct
{
   /// @brief Mesh to render. 
    blxMesh mesh;

    /// @brief Material to use when rendering these meshes.
    blxMaterial* material;

     /// @brief Transform to apply to the geometry.
    blxTransform* transform;
}blxRenderableGeometry;

