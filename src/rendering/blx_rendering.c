#include "blx_rendering.h"
#include "GL/glew.h"
#include <malloc.h>
#include "internal/opengl.h"
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "cglm/struct/vec3.h"
#include "cglm/struct/vec2.h"
#include "utils/blx_hashTable.h"
#include "core/blx_logger.h"
#include "core/blx_memory.h"
#include "rendering/blx_material.h"
#include "rendering/blx_shader.h"
#include "utils/blx_vlist.h"

typedef struct
{
    void (*Init)();
    void (*Draw)(blxRenderPacket* packet);
    void (*UpdateMesh)(blxMesh*);
    void (*InitMesh)(blxMesh*);
    void (*SetShadingMode) (blxShadingMode mode);
    void (*RegisterBatch) (MaterialGroup* matGroup);
    blxRenderPacket packet;
} blxRenderer;



blxRenderer* renderer;
static blxBool initialized = BLX_FALSE;
static blxMesh defaultUIGeometry;

#include "utils/blx_assertions.h"

// TODO: Move this to logger! ----
void GLDebug(GLenum source, GLenum type, GLuint id,
    GLenum severity, GLsizei length,
    const GLchar* msg, const void* data)
{
    // printf("-------\n");
    // printf("%s\n", msg);
    // printf("%s %d\n", __FILE__, __LINE__);
    // printf("-------\n");

    //BLXASSERT(BLX_FALSE);
}
// -------


void blxInitRenderer(GraphicsAPI graphicsToUse)
{
    if (!initialized) {
        //Instead of malloc might be better to just have a static state.
        renderer = (blxRenderer*)blxAllocate(sizeof(blxRenderer), BLXMEMORY_TAG_RENDERER);
        blxZeroMemory(&renderer->packet, sizeof(blxRenderPacket));
        //TODO: TEMP FOR NOW TILL WE GET A MATERIAL SYSTEM.
        renderer->packet.directionalLight.diffuse = (vec3s){ 1.0f, 1.0f, 1.0f };
        //TODO: Dynamic allocator / Free List.
        renderer->packet.vlist_materialGroups = blxInitListWithSize(MaterialGroup, 32);
        initialized = BLX_TRUE;
        _blxInitMaterialSystem();
    }

    switch (graphicsToUse)
    {
        case OPENGL:
            renderer->Draw = blxGL_Draw;
            renderer->Init = blxGL_Init;
            renderer->InitMesh = blxGL_InitMesh;
            renderer->UpdateMesh = OpenGLUpdateMesh;
            renderer->SetShadingMode = blxGL_SetShadingMode;
            renderer->RegisterBatch = blxGL_RegisterBatch;
            glEnable(GL_DEBUG_OUTPUT);
            glDebugMessageCallback(GLDebug, 0);
            BLXINFO("Initializing Renderer with OpenGL...");
            break;
    }

    renderer->Init();
    renderer->InitMesh(&defaultUIGeometry);
    //TODO: Make these vertices and indices part of a built in obj file.
    defaultUIGeometry.vertices = blxInitListWithSize(vList_blxVertex, 4);
    defaultUIGeometry.indices = blxInitListWithSize(vList_indices, 6);

    blxAddValueToList(defaultUIGeometry.vertices, ((blxVertex)
        //position
    {   0.5f, 0.5f, 0.0f,
        //normal
        0.0f, 0.0f, 0.0f,
        //uv
        1.0f, 1.0f
    }));
    blxAddValueToList(defaultUIGeometry.vertices, ((blxVertex)
        //position
    {   0.5f, -0.5f, 0.0f,
        //normal
        0.0f, 0.0f, 0.0f,
        //uv
        1.0f, 0.0f
    }));
    blxAddValueToList(defaultUIGeometry.vertices, ((blxVertex)
        //position
    {   -0.5f, -0.5f, 0.0f,
        //normal
        0.0f, 0.0f, 0.0f,
        //uv
        0.0f, 0.0f
    }));
    blxAddValueToList(defaultUIGeometry.vertices, ((blxVertex)
        //position
    {   -0.5f, 0.5f, 0.0f,
        //normal
        0.0f, 0.0f, 0.0f,
        //uv
        0.0f, 1.0f
    }));

    blxAddValueToList(defaultUIGeometry.indices, 0);
    blxAddValueToList(defaultUIGeometry.indices, 1);
    blxAddValueToList(defaultUIGeometry.indices, 3);
    blxAddValueToList(defaultUIGeometry.indices, 1);
    blxAddValueToList(defaultUIGeometry.indices, 2);
    blxAddValueToList(defaultUIGeometry.indices, 3);
    //End TODO
    renderer->UpdateMesh(&defaultUIGeometry);


    _blxShader_SystemInitialize(graphicsToUse);
}


void blxAddCameraToRender(Camera* cam)
{
    renderer->packet.cam = cam;
}

void blxDrawModel(blxModel* model)
{

    //TODO: This needs to be refactored!
    for (unsigned int i = 0; i < blxGetListCount(model->geometries); i++)
    {
        unsigned int id = model->geometries[i].material->id;
        MaterialGroup* group = &renderer->packet.vlist_materialGroups[model->geometries[i].material->id];

        blxMesh mesh = model->geometries[i].mesh;
        //TODO: pointers to a mesh should be added here instead of copying the whole mesh itself..
        blxAddValueToList(group->vlist_meshes, mesh);

        unsigned int c = _BLX_GET_HEADER(group->indices, _BLX_MAX_LENGTH_I);

        blxAddArrayToList(group->indices, mesh.indices, blxGetListCount(mesh.indices));
        blxAddArrayToList(group->vertices, mesh.vertices, blxGetListCount(mesh.vertices));

        c = _BLX_GET_HEADER(group->indices, _BLX_MAX_LENGTH_I);

        blxAddValueToList(group->geometries, model->geometries[i]);

    }

    //blxAddValueToList(renderer->packet.modelsList, model);
}

void blxDrawUI(blxModel* model)
{
    renderer->packet.ui[renderer->packet.uiCount] = *model;
    renderer->packet.uiCount += 1;
}

void blxInitMesh(blxMesh* mesh)
{
    renderer->InitMesh(mesh);
}

void blxSetShadingMode(blxShadingMode mode)
{
    renderer->SetShadingMode(mode);
}

void blxDraw()
{
    camera_update(renderer->packet.cam);
    renderer->Draw(&renderer->packet);
    renderer->packet.uiCount = 0;
    for (uint64 i = 0; i < blxGetListCount(renderer->packet.vlist_materialGroups); i++)
    {
        MaterialGroup group = renderer->packet.vlist_materialGroups[i];
        blxClearList(group.vlist_meshes);
        blxClearList(group.indices);
        blxClearList(group.vertices);
        blxClearList(group.geometries);
    }
}

void blxUpdateMesh(blxMesh* mesh)
{
    renderer->UpdateMesh(mesh);
}

void _blxRendererRegisterMaterial(blxMaterial* material)
{
    MaterialGroup matGroup;
    matGroup.material = material;
    matGroup.material->id = blxGetListCount(renderer->packet.vlist_materialGroups);
    matGroup.vlist_meshes = blxInitList(blxMesh);
    matGroup.vertices = blxInitList(blxVertex);
    matGroup.indices = blxInitList(unsigned int);
    matGroup.geometries = blxInitList(blxRenderableGeometry);
    blxAddValueToList(renderer->packet.vlist_materialGroups, matGroup);
    renderer->RegisterBatch(&renderer->packet.vlist_materialGroups[matGroup.material->id]);
}

// TODO: Move this into a obj.c file for loading obj files...
//---------------------------------------------------------------

// typedef struct {
//     unsigned int posIndex, texIndex, normIndex;
// }ObjVertex;

// blxBool VertexKeyCompare(void* a, void* b)
// {
//     blxVertex* vertA = (blxVertex*)a;
//     blxVertex* vertB = (blxVertex*)b;

//     if (!glms_vec3_eqv(vertA->position, vertB->position)) {
//         return BLX_FALSE;
//     }

//     if (!glms_vec3_eqv(vertA->normal, vertB->normal)) {
//         return BLX_FALSE;
//     }

//     if (!glms_vec2_eqv(vertA->uv, vertB->uv)) {
//         return BLX_FALSE;
//     }

//     return BLX_TRUE;
// }

// void blxImportMesh(const char* filePath, blxMesh* outMesh)
// {

//     // TODO: CLEAN THIS OUT AND OPTIMIZE LOADING, OPTIONALLY MAKE THIS MULTITHREADED.
//     blxInitMesh(outMesh);
//     blxHashTable* table = blxCreateHashTable(blxVertex, unsigned int, VertexKeyCompare);
//     outMesh->vertices = blxInitList(vList_blxVertex);
//     outMesh->indices = blxInitList(vList_indices);
//     vec3s* positions = blxInitList(vec3s);
//     vec3s* normals = blxInitList(vec3s);
//     vec2s* texCoords = blxInitList(vec2s);
//     FILE* file;
//     file = fopen(filePath, "r");
//     char lineBuffer[1024];
//     while (fgets(lineBuffer, sizeof(lineBuffer), file))
//     {
//         //printf("%s", lineBuffer);
//         char firstChar = lineBuffer[0];
//         switch (firstChar)
//         {
//             case '#': // Skip Comments
//                 continue;
//                 break;
//             case 'v':
//                 char secondChar = lineBuffer[1];
//                 switch (secondChar)
//                 {
//                     case ' ': { // Vertex Position
//                         vec3s pos;
//                         char temp[2];
//                         sscanf(lineBuffer, "%s %f %f %f", temp, &pos.x, &pos.y, &pos.z);
//                         blxAddValueToList(positions, pos);
//                     }break;

//                     case 'n': { //Vertex Normal
//                         vec3s normal;
//                         char temp[2];
//                         sscanf(lineBuffer, "%s %f %f %f", temp, &normal.x, &normal.y, &normal.z);
//                         blxAddValueToList(normals, normal);
//                     }break;

//                     case 't': { // Vertex Tex Coord
//                         vec2s uv;
//                         char temp[2];
//                         sscanf(lineBuffer, "%s %f %f", temp, &uv.x, &uv.y);
//                         blxAddValueToList(texCoords, uv);
//                     }break;

//                 }

//                 break;

//             case 'f': {
//                 // face indices.
//                 // 1/1/1 2/2/2 3/3/3 = pos/tex/norm pos/tex/norm pos/tex/norm
//                 char temp[2];
//                 vec3s triangle;
//                 ObjVertex v1, v2, v3, v4;
//                 int filled = sscanf(lineBuffer, "%s %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", temp,
//                     &v1.posIndex, &v1.texIndex, &v1.normIndex,
//                     &v2.posIndex, &v2.texIndex, &v2.normIndex,
//                     &v3.posIndex, &v3.texIndex, &v3.normIndex,
//                     &v4.posIndex, &v4.texIndex, &v4.normIndex);
//                 //PRINTVEC3S(triangle);
//                 printf("%d\n", filled);
//                 //printf("Pos Index: %d Tex Index: %d Norm Index: %d\n", v1.posIndex, v1.texIndex, v1.normIndex);
//                 if (filled == 10) //if 10 items filled from sscanf then we have a triangulated mesh.
//                 {
//                     //3 Vertices for a triangle.
//                     blxVertex triangleP1, triangleP2, triangleP3;

//                     //substract from 1 due to obj indices starting at 1. 
//                     glm_vec3_copy(positions[v1.posIndex - 1].raw, triangleP1.position.raw);
//                     glm_vec2_copy(texCoords[v1.texIndex - 1].raw, triangleP1.uv.raw);
//                     glm_vec3_copy(normals[v1.normIndex - 1].raw, triangleP1.normal.raw);

//                     glm_vec3_copy(positions[v2.posIndex - 1].raw, triangleP2.position.raw);
//                     glm_vec2_copy(texCoords[v2.texIndex - 1].raw, triangleP2.uv.raw);
//                     glm_vec3_copy(normals[v2.normIndex - 1].raw, triangleP2.normal.raw);

//                     vec3s test3 = positions[v3.posIndex - 1];
//                     glm_vec3_copy(test3.raw, triangleP3.position.raw);
//                     glm_vec2_copy(texCoords[v3.texIndex - 1].raw, triangleP3.uv.raw);
//                     glm_vec3_copy(normals[v3.normIndex - 1].raw, triangleP3.normal.raw);

//                     unsigned int vertexIndex;
//                     blxAddValueToList(outMesh->vertices, triangleP1);
//                     // vertexIndex = blxGetListCount(outMesh->vertices) - 1;
//                     // blxAddValueToList(outMesh->indices, vertexIndex);

//                     if (blxHashTableKeyExist(table, &triangleP1, &vertexIndex)) {
//                         printf("HASH VALUE VERTEX INDEX!!!: %d\n", vertexIndex);
//                         blxAddValueToList(outMesh->indices, vertexIndex);
//                     }
//                     else {
//                         vertexIndex = blxGetListCount(outMesh->vertices) - 1;
//                         blxAddToHashTableAlloc(table, triangleP1, vertexIndex);
//                         blxAddValueToList(outMesh->indices, vertexIndex);
//                     }

//                     blxAddValueToList(outMesh->vertices, triangleP2);
//                     // vertexIndex = blxGetListCount(outMesh->vertices) - 1;
//                     // blxAddValueToList(outMesh->indices, vertexIndex);

//                     if (blxHashTableKeyExist(table, &triangleP2, &vertexIndex)) {
//                         printf("HASH VALUE VERTEX INDEX!!!: %d\n", vertexIndex);
//                         blxAddValueToList(outMesh->indices, vertexIndex);
//                     }
//                     else {
//                         vertexIndex = blxGetListCount(outMesh->vertices) - 1;
//                         blxAddToHashTableAlloc(table, triangleP2, vertexIndex);
//                         blxAddValueToList(outMesh->indices, vertexIndex);
//                     }

//                     blxAddValueToList(outMesh->vertices, triangleP3);
//                     // vertexIndex = blxGetListCount(outMesh->vertices) - 1;
//                     // blxAddValueToList(outMesh->indices, vertexIndex);

//                     if (blxHashTableKeyExist(table, &triangleP3, &vertexIndex)) {
//                         printf("HASH VALUE VERTEX INDEX!!!: %d\n", vertexIndex);
//                         blxAddValueToList(outMesh->indices, vertexIndex);
//                     }
//                     else {
//                         vertexIndex = blxGetListCount(outMesh->vertices) - 1;
//                         blxAddToHashTableAlloc(table, triangleP3, vertexIndex);
//                         blxAddValueToList(outMesh->indices, vertexIndex);
//                     }

//                     for (size_t i = 0; i < 3; i++)
//                     {
//                         if (i == 0)
//                         {
//                             printf("Pos Index: %d Tex Index: %d Norm Index: %d\n", v1.posIndex, v1.texIndex, v1.normIndex);
//                         }

//                         if (i == 1)
//                         {
//                             printf("Pos Index: %d Tex Index: %d Norm Index: %d\n", v2.posIndex, v2.texIndex, v2.normIndex);
//                         }

//                         if (i == 2)
//                         {
//                             printf("Pos Index: %d Tex Index: %d Norm Index: %d\n", v3.posIndex, v3.texIndex, v3.normIndex);
//                         }
//                     }

//                     printf("------------------\n");
//                     PRINTVEC3(triangleP1.position.raw);
//                     PRINTVEC2(triangleP1.uv.raw);
//                     PRINTVEC3(triangleP1.normal.raw);
//                     printf("------------------\n");
//                     PRINTVEC3(triangleP2.position.raw);
//                     PRINTVEC2(triangleP2.uv.raw);
//                     PRINTVEC3(triangleP2.normal.raw);
//                     printf("------------------\n");
//                     PRINTVEC3(triangleP3.position.raw);
//                     PRINTVEC2(triangleP3.uv.raw);
//                     PRINTVEC3(triangleP3.normal.raw);
//                     printf("------------------\n");
//                 }

//                 //Not a triangulated mesh instead we are working with quads.
//                 else if (filled == 13)
//                 {
//                     //Split 1 quad into 2 triangles.
//                     blxVertex quadP1, quadP2, quadP3, quadP4;
//                     glm_vec3_copy(positions[v1.posIndex - 1].raw, quadP1.position.raw);
//                     glm_vec2_copy(texCoords[v1.texIndex - 1].raw, quadP1.uv.raw);
//                     glm_vec3_copy(normals[v1.normIndex - 1].raw, quadP1.normal.raw);

//                     glm_vec3_copy(positions[v2.posIndex - 1].raw, quadP2.position.raw);
//                     glm_vec2_copy(texCoords[v2.texIndex - 1].raw, quadP2.uv.raw);
//                     glm_vec3_copy(normals[v2.normIndex - 1].raw, quadP2.normal.raw);

//                     glm_vec3_copy(positions[v3.posIndex - 1].raw, quadP3.position.raw);
//                     glm_vec2_copy(texCoords[v3.texIndex - 1].raw, quadP3.uv.raw);
//                     glm_vec3_copy(normals[v3.normIndex - 1].raw, quadP3.normal.raw);

//                     glm_vec3_copy(positions[v4.posIndex - 1].raw, quadP4.position.raw);
//                     glm_vec2_copy(texCoords[v4.texIndex - 1].raw, quadP4.uv.raw);
//                     glm_vec3_copy(normals[v4.normIndex - 1].raw, quadP4.normal.raw);

//                     unsigned int vertexIndex = blxGetListCount(outMesh->vertices);

//                     //First Triangle ---------------
//                     blxAddValueToList(outMesh->vertices, quadP1);
//                     blxAddValueToList(outMesh->indices, vertexIndex);

//                     vertexIndex++;
//                     blxAddValueToList(outMesh->vertices, quadP2);
//                     blxAddValueToList(outMesh->indices, vertexIndex);

//                     vertexIndex++;
//                     blxAddValueToList(outMesh->vertices, quadP3);
//                     blxAddValueToList(outMesh->indices, vertexIndex);
//                     //-----------

//                     //Second Triangle -------------
//                     //blxAddValueToList(outMesh->vertices, quadP1);
//                     blxAddValueToList(outMesh->indices, vertexIndex - 2);

//                     //blxAddValueToList(outMesh->vertices, quadP3);
//                     blxAddValueToList(outMesh->indices, vertexIndex);
//                     vertexIndex++;
//                     blxAddValueToList(outMesh->vertices, quadP4);
//                     blxAddValueToList(outMesh->indices, vertexIndex);
//                    // vertexIndex++;
//                    // blxAddValueToList(outMesh->indices, vertexIndex);
//                    //-----------
//                 }

//             } break;

//         }
//     }
//     blxUpdateMesh(outMesh);
//     printf("Indices list count: %d\n", blxGetListCount(outMesh->indices));
//     printf("Vertex count %d\n", blxGetListCount(outMesh->vertices));
//     for (size_t i = 0; i < blxGetListCount(outMesh->vertices); i++)
//     {
//         vec3s pos = outMesh->vertices[i].position;
//         printf("Vertex pos at Index: %d with mesh index being: %d: (%f, %f, %f)\n", i, outMesh->indices[i], pos.raw[0], pos.raw[1], pos.raw[2]);
//     }

//     // for (size_t i = 0; i < blxGetListCount(texCoords); i++)
//     // {
//     //     PRINTVEC2S(texCoords[i]);
//     // }
//     blxFreeHashTable(table);
// }

//---------------------------------------------------------------


