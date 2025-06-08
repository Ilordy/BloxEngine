#include "core/loaders/blx_objLoader.h"
#include "blx_objLoader.h"
#include "rendering/blx_rendering.h"
#include "utils/blx_hashTable.h"
#include "utils/blx_fileManagement.h"
#include "core/blx_string.h"
#include "utils/blx_vlist.h"
#include "cglm/struct/vec2-ext.h"
//TODO: Move into resource system/ resource loader.

typedef struct {
    unsigned int posIndex, texIndex, normIndex;
}ObjVertex;

blxBool VertexKeyCompare(void* a, void* b)
{
    blxVertex* vertA = (blxVertex*)a;
    blxVertex* vertB = (blxVertex*)b;

    if (!glms_vec3_eqv(vertA->position, vertB->position)) {
        return BLX_FALSE;
    }

    if (!glms_vec3_eqv(vertA->normal, vertB->normal)) {
        return BLX_FALSE;
    }

    if (!glms_vec2_eqv(vertA->uv, vertB->uv)) {
        return BLX_FALSE;
    }

    return BLX_TRUE;
}

static blxBool StrKeyCompare(void* a, void* b)
{
    return blxStr_Cmp((char*)a, (char*)b);
}

// TODO: This should be in the blxString header file.
static uint64 StrToHash(void* key) {
    return blxToHash(key, blxStr_Len(key));
}

static blxHashTable* ReadMtlFile(const char* mtlPath)
{
    blxFile* mtlFile;
    if (!blxOpenFile(mtlPath, BLX_FILE_MODE_READ, &mtlFile)) {
        return NULL;
        
    }
    char lineBuffer[512];
    char* p = &lineBuffer;
    uint64 lineLength;
    blxHashTable* matTable = blxCreateHashTableWithHash(const char*, blxMaterial*, StrKeyCompare, StrToHash);

    blxMaterial* mat;

    while (blxFileReadLine(mtlFile, 1024, &p, &lineLength))
    {
        //const char* newmtlOffset = blxStrFindSubStr(lineBuffer, "newmtl");
        char firstChar = lineBuffer[0];

        switch (firstChar)
        {
            case 'n': {
                mat = blxMaterial_CreateDefault();
                char matName[128];

                //offset by 7 for the newmtl keyword and space.
                blxStr_Cpy(matName, lineBuffer + 7);

                blxAddToHashTableAllocA(matTable, matName, mat);
            }break;
            case 'K': {
                char secondChar = lineBuffer[1];
                switch (secondChar)
                {
                    //diffuse color
                    case 'd': {
                        vec4s diffuseColor;
                        char* diffuseStart = lineBuffer + 3;

                        sscanf(diffuseStart, "%f %f %f", &diffuseColor.r, &diffuseColor.g, &diffuseColor.b);
                        diffuseColor.a = 1;

                        blxMaterial_SetValue(mat, "baseColor", BLX_MAT_PROP_VEC4, &diffuseColor);
                    }break;
                }
            }break;
        }
    }

    blxCloseFile(mtlFile);
    return matTable;
}


void blxImportModelFromObj(blxModel* outModel, const char* objPath)
{
    blxFile* objFile;
    if (!blxOpenFile(objPath, BLX_FILE_MODE_READ, &objFile))
    {
        return;
    }

    blxModel_Init(outModel);

    blxAddValueToList(outModel->geometries, (blxRenderableGeometry) { 0 });
    // Initialize geometry pointer to the first element in the list.
    // For clarity we are setting it to the address after defrencing the pointer of the first element.
    blxRenderableGeometry* geoPtr = &outModel->geometries[0];


    geoPtr->mesh.vertices = blxInitList(vList_blxVertex);
    geoPtr->mesh.indices = blxInitList(vList_indices);

    geoPtr->material = NULL;
    geoPtr->transform = &outModel->transform;

    blxHashTable* table = blxCreateHashTable(blxVertex, unsigned int, VertexKeyCompare);
    blxHashTable* matTable = NULL;

    // Reserve some initial space to reduce reallocations.
    // 4096 is a reasonable default for small/medium models, but you can tune as needed.
    vec3s* positions = blxInitListWithSize(vec3s, 4096);
    vec3s* normals = blxInitListWithSize(vec3s, 4096);
    vec2s* texCoords = blxInitListWithSize(vec2s, 4096);

    char lineBuffer[512];
    char* p = &lineBuffer;
    uint64 lineLength;
    while (blxFileReadLine(objFile, 1024, &p, &lineLength))
    {
        char firstChar = lineBuffer[0];
        BLXDEBUG("%s", lineBuffer);
        switch (firstChar)
        {
            case '#': // Skip Comments
                continue;
                break;
            case 'o': {
                //TODO: Add support for importing multiple objects under one "empty parent"!
            }break;
            case 'v':
                char secondChar = lineBuffer[1];
                switch (secondChar)
                {
                    case ' ': { // Vertex Position
                        vec3s pos;
                        char temp[2];
                        sscanf(lineBuffer, "%s %f %f %f", temp, &pos.x, &pos.y, &pos.z);
                        blxAddValueToList(positions, pos);
                    }break;

                    case 'n': { //Vertex Normal
                        vec3s normal;
                        char temp[2];
                        sscanf(lineBuffer, "%s %f %f %f", temp, &normal.x, &normal.y, &normal.z);
                        blxAddValueToList(normals, normal);
                    }break;

                    case 't': { // Vertex Tex Coord
                        vec2s uv;
                        char temp[2];
                        sscanf(lineBuffer, "%s %f %f", temp, &uv.x, &uv.y);
                        blxAddValueToList(texCoords, uv);
                    }break;

                }

                break;

            case 'f': {
                // face indices.
                // 1/1/1 2/2/2 3/3/3 = pos/tex/norm pos/tex/norm pos/tex/norm
                char temp[2];
                vec3s triangle;
                ObjVertex v1, v2, v3, v4;
                int filled = sscanf(lineBuffer, "%s %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", temp,
                    &v1.posIndex, &v1.texIndex, &v1.normIndex,
                    &v2.posIndex, &v2.texIndex, &v2.normIndex,
                    &v3.posIndex, &v3.texIndex, &v3.normIndex,
                    &v4.posIndex, &v4.texIndex, &v4.normIndex);
                //PRINTVEC3S(triangle);
                printf("%d\n", filled);
                //printf("Pos Index: %d Tex Index: %d Norm Index: %d\n", v1.posIndex, v1.texIndex, v1.normIndex);
                if (filled == 10) //if 10 items filled from sscanf then we have a triangulated mesh.
                {
                    //3 Vertices for a triangle.
                    blxVertex triangleP1, triangleP2, triangleP3;

                    //substract from 1 due to obj indices starting at 1. 
                    glm_vec3_copy(positions[v1.posIndex - 1].raw, triangleP1.position.raw);
                    glm_vec2_copy(texCoords[v1.texIndex - 1].raw, triangleP1.uv.raw);
                    glm_vec3_copy(normals[v1.normIndex - 1].raw, triangleP1.normal.raw);

                    glm_vec3_copy(positions[v2.posIndex - 1].raw, triangleP2.position.raw);
                    glm_vec2_copy(texCoords[v2.texIndex - 1].raw, triangleP2.uv.raw);
                    glm_vec3_copy(normals[v2.normIndex - 1].raw, triangleP2.normal.raw);

                    vec3s test3 = positions[v3.posIndex - 1];
                    glm_vec3_copy(test3.raw, triangleP3.position.raw);
                    glm_vec2_copy(texCoords[v3.texIndex - 1].raw, triangleP3.uv.raw);
                    glm_vec3_copy(normals[v3.normIndex - 1].raw, triangleP3.normal.raw);

                    unsigned int vertexIndex;
                    blxAddValueToList(geoPtr->mesh.vertices, triangleP1);
                    // vertexIndex = blxGetListCount(geoPtr->mesh.vertices) - 1;
                    // blxAddValueToList(geoPtr->mesh.indices, vertexIndex);

                    if (blxHashTableKeyExist(table, &triangleP1, &vertexIndex)) {
                        printf("HASH VALUE VERTEX INDEX!!!: %d\n", vertexIndex);
                        blxAddValueToList(geoPtr->mesh.indices, vertexIndex);
                    }
                    else {
                        vertexIndex = blxGetListCount(geoPtr->mesh.vertices) - 1;
                        blxAddToHashTableAlloc(table, triangleP1, vertexIndex);
                        blxAddValueToList(geoPtr->mesh.indices, vertexIndex);
                    }

                    blxAddValueToList(geoPtr->mesh.vertices, triangleP2);
                    // vertexIndex = blxGetListCount(geoPtr->mesh.vertices) - 1;
                    // blxAddValueToList(geoPtr->mesh.indices, vertexIndex);

                    if (blxHashTableKeyExist(table, &triangleP2, &vertexIndex)) {
                        printf("HASH VALUE VERTEX INDEX!!!: %d\n", vertexIndex);
                        blxAddValueToList(geoPtr->mesh.indices, vertexIndex);
                    }
                    else {
                        vertexIndex = blxGetListCount(geoPtr->mesh.vertices) - 1;
                        blxAddToHashTableAlloc(table, triangleP2, vertexIndex);
                        blxAddValueToList(geoPtr->mesh.indices, vertexIndex);
                    }

                    blxAddValueToList(geoPtr->mesh.vertices, triangleP3);
                    // vertexIndex = blxGetListCount(geoPtr->mesh.vertices) - 1;
                    // blxAddValueToList(geoPtr->mesh.indices, vertexIndex);

                    if (blxHashTableKeyExist(table, &triangleP3, &vertexIndex)) {
                        printf("HASH VALUE VERTEX INDEX!!!: %d\n", vertexIndex);
                        blxAddValueToList(geoPtr->mesh.indices, vertexIndex);
                    }
                    else {
                        vertexIndex = blxGetListCount(geoPtr->mesh.vertices) - 1;
                        blxAddToHashTableAlloc(table, triangleP3, vertexIndex);
                        blxAddValueToList(geoPtr->mesh.indices, vertexIndex);
                    }

                    for (size_t i = 0; i < 3; i++)
                    {
                        if (i == 0)
                        {
                            printf("Pos Index: %d Tex Index: %d Norm Index: %d\n", v1.posIndex, v1.texIndex, v1.normIndex);
                        }

                        if (i == 1)
                        {
                            printf("Pos Index: %d Tex Index: %d Norm Index: %d\n", v2.posIndex, v2.texIndex, v2.normIndex);
                        }

                        if (i == 2)
                        {
                            printf("Pos Index: %d Tex Index: %d Norm Index: %d\n", v3.posIndex, v3.texIndex, v3.normIndex);
                        }
                    }

                    printf("------------------\n");
                    PRINTVEC3(triangleP1.position.raw);
                    PRINTVEC2(triangleP1.uv.raw);
                    PRINTVEC3(triangleP1.normal.raw);
                    printf("------------------\n");
                    PRINTVEC3(triangleP2.position.raw);
                    PRINTVEC2(triangleP2.uv.raw);
                    PRINTVEC3(triangleP2.normal.raw);
                    printf("------------------\n");
                    PRINTVEC3(triangleP3.position.raw);
                    PRINTVEC2(triangleP3.uv.raw);
                    PRINTVEC3(triangleP3.normal.raw);
                    printf("------------------\n");
                }

                //Not a triangulated mesh instead we are working with quads.
                else if (filled == 13)
                {
                    //Split 1 quad into 2 triangles.
                    blxVertex quadP1, quadP2, quadP3, quadP4;
                    glm_vec3_copy(positions[v1.posIndex - 1].raw, quadP1.position.raw);
                    glm_vec2_copy(texCoords[v1.texIndex - 1].raw, quadP1.uv.raw);
                    glm_vec3_copy(normals[v1.normIndex - 1].raw, quadP1.normal.raw);

                    glm_vec3_copy(positions[v2.posIndex - 1].raw, quadP2.position.raw);
                    glm_vec2_copy(texCoords[v2.texIndex - 1].raw, quadP2.uv.raw);
                    glm_vec3_copy(normals[v2.normIndex - 1].raw, quadP2.normal.raw);

                    glm_vec3_copy(positions[v3.posIndex - 1].raw, quadP3.position.raw);
                    glm_vec2_copy(texCoords[v3.texIndex - 1].raw, quadP3.uv.raw);
                    glm_vec3_copy(normals[v3.normIndex - 1].raw, quadP3.normal.raw);

                    glm_vec3_copy(positions[v4.posIndex - 1].raw, quadP4.position.raw);
                    glm_vec2_copy(texCoords[v4.texIndex - 1].raw, quadP4.uv.raw);
                    glm_vec3_copy(normals[v4.normIndex - 1].raw, quadP4.normal.raw);

                    unsigned int vertexIndex = blxGetListCount(geoPtr->mesh.vertices);

                    //First Triangle ---------------
                    blxAddValueToList(geoPtr->mesh.vertices, quadP1);
                    blxAddValueToList(geoPtr->mesh.indices, vertexIndex);

                    vertexIndex++;
                    blxAddValueToList(geoPtr->mesh.vertices, quadP2);
                    blxAddValueToList(geoPtr->mesh.indices, vertexIndex);

                    vertexIndex++;
                    blxAddValueToList(geoPtr->mesh.vertices, quadP3);
                    blxAddValueToList(geoPtr->mesh.indices, vertexIndex);
                    //-----------

                    //Second Triangle -------------
                    //blxAddValueToList(geoPtr->mesh.vertices, quadP1);
                    blxAddValueToList(geoPtr->mesh.indices, vertexIndex - 2);

                    //blxAddValueToList(geoPtr->mesh.vertices, quadP3);
                    blxAddValueToList(geoPtr->mesh.indices, vertexIndex);
                    vertexIndex++;
                    blxAddValueToList(geoPtr->mesh.vertices, quadP4);
                    blxAddValueToList(geoPtr->mesh.indices, vertexIndex);
                   // vertexIndex++;
                   // blxAddValueToList(geoPtr->mesh.indices, vertexIndex);
                   //--------

                }

                //Keep track of the current file position.
                // filePos = blxFileGetPos(objFile);
                // int length = strlen(lineBuffer);
                // if (blxFileReadLine(objFile, 1024, &p, &lineLength))
                // {
                //     firstChar = lineBuffer[0];
                //     if (firstChar != 'f')
                //     {
                //         //Set file position back to the last line read.
                //         BLXDEBUG("%s", lineBuffer);
                //         blxFileSetPos(objFile, &filePos);
                //         blxFileReadLine(objFile, 1024, &p, &lineLength);
                //         BLXDEBUG("%s", lineBuffer);

                //     }
                // }
                // else
                // {
                //     break;
                // }

            } break;

            case 'u': {

                //TODO: Verify if we need this check.
                if (lineLength > 6)
                {

                    // if (geoPtr->material != NULL)
                    // {
                    //     blxAddValueToList(outModel->geometries, geometry);
                    //     geoPtr->mesh.vertices = blxInitList(vList_blxVertex);
                    //     geoPtr->mesh.indices = blxInitList(vList_indices);
                    // }

                    //allocate 7 bytes to include the null terminator.
                    char mtlTok[7];
                    blxStrn_Cpy(mtlTok, lineBuffer, 6);
                    if (blxStr_Cmp(mtlTok, "usemtl")) {

                        // The first geometry will not have a material till after the second iteration.
                        if (geoPtr->material != NULL)
                        {
                            blxAddValueToList(outModel->geometries, (blxRenderableGeometry) { 0 });
                            geoPtr = &outModel->geometries[blxGetListCount(outModel->geometries) - 1];
                            geoPtr->mesh.vertices = blxInitList(vList_blxVertex);
                            geoPtr->mesh.indices = blxInitList(vList_indices);
                            geoPtr->transform = &outModel->transform;
                            //TODO: Instead of freeing we should just reuse the memory again. maybe with an arena...
                            blxFreeHashTable(table);
                            table = blxCreateHashTable(blxVertex, unsigned int, VertexKeyCompare);
                        }

                        //Assuming mat name is less than 64..
                        char matName[65];
                        blxStr_Cpy(matName, lineBuffer + 7);

                        blxMaterial* mat;
                        if (matTable && blxHashTableKeyExist(matTable, matName, &mat)) {
                            geoPtr->material = mat;
                        }
                        else {
                            BLXWARNING("Material %s not found in material table.", matName);
                            geoPtr->material = blxMaterial_CreateDefault();
                        }
                    }
                }
            }break;

            //Assuming this is a mttlib
            case 'm': {
                //Assume the mttlib is in the same path as the obj.
                int periodIndex = blxStr_IndexOfLastChar(objPath, '.');
                char mtlFileName[blxMaxFilePath];
                blxStr_Cpy(mtlFileName, objPath);

                mtlFileName[periodIndex + 1] = 'm';
                mtlFileName[periodIndex + 2] = 't';
                mtlFileName[periodIndex + 3] = 'l';

                matTable = ReadMtlFile(mtlFileName);
                if (!matTable)
                {
                    BLXWARNING("Material file not found for obj %s\n Using default material.", objPath);
                }

            }break;
        }
    }

    // If no material file was found that means there's only one piece of geometry in the obj file.
    // Set the only geometry's material to the default material.
    if (!matTable)
    {
        geoPtr->material = blxMaterial_CreateDefault();
    }
    else{
        blxFreeHashTable(matTable);
    }


    // Free the allocated lists and hash tables.
    blxFreeList(positions);
    blxFreeList(normals);
    blxFreeList(texCoords);
    blxFreeHashTable(table);

    // Close the file after reading.
    blxCloseFile(objFile);
}