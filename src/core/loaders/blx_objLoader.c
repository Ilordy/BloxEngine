#include "core/loaders/blx_objLoader.h"
#include "blx_objLoader.h"
#include "rendering/blx_rendering.h"
#include "utils/blx_hashTable.h"
#include "utils/blx_fileManagement.h"
#include "core/blx_string.h"
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


static blxBool ExtractMesh(blxMesh* outMesh, blxFile* objFile)
{
    //blxInitMesh(outMesh);
    blxHashTable* table = blxCreateHashTable(blxVertex, unsigned int, VertexKeyCompare);

    outMesh->vertices = blxInitList(vList_blxVertex);
    outMesh->indices = blxInitList(vList_indices);
    vec3s* positions = blxInitList(vec3s);
    vec3s* normals = blxInitList(vec3s);
    vec2s* texCoords = blxInitList(vec2s);

    char lineBuffer[512];
    char* p = &lineBuffer;
    uint64 lineLength;
    while (blxFileReadLine(objFile, 1024, &p, &lineLength))
    {
        char firstChar = lineBuffer[0];
        switch (firstChar)
        {
            case '#': // Skip Comments
                continue;
                break;
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
                    blxAddValueToList(outMesh->vertices, triangleP1);
                    // vertexIndex = blxGetListCount(outMesh->vertices) - 1;
                    // blxAddValueToList(outMesh->indices, vertexIndex);

                    if (blxHashTableKeyExist(table, &triangleP1, &vertexIndex)) {
                        printf("HASH VALUE VERTEX INDEX!!!: %d\n", vertexIndex);
                        blxAddValueToList(outMesh->indices, vertexIndex);
                    }
                    else {
                        vertexIndex = blxGetListCount(outMesh->vertices) - 1;
                        blxAddToHashTableAlloc(table, triangleP1, vertexIndex);
                        blxAddValueToList(outMesh->indices, vertexIndex);
                    }

                    blxAddValueToList(outMesh->vertices, triangleP2);
                    // vertexIndex = blxGetListCount(outMesh->vertices) - 1;
                    // blxAddValueToList(outMesh->indices, vertexIndex);

                    if (blxHashTableKeyExist(table, &triangleP2, &vertexIndex)) {
                        printf("HASH VALUE VERTEX INDEX!!!: %d\n", vertexIndex);
                        blxAddValueToList(outMesh->indices, vertexIndex);
                    }
                    else {
                        vertexIndex = blxGetListCount(outMesh->vertices) - 1;
                        blxAddToHashTableAlloc(table, triangleP2, vertexIndex);
                        blxAddValueToList(outMesh->indices, vertexIndex);
                    }

                    blxAddValueToList(outMesh->vertices, triangleP3);
                    // vertexIndex = blxGetListCount(outMesh->vertices) - 1;
                    // blxAddValueToList(outMesh->indices, vertexIndex);

                    if (blxHashTableKeyExist(table, &triangleP3, &vertexIndex)) {
                        printf("HASH VALUE VERTEX INDEX!!!: %d\n", vertexIndex);
                        blxAddValueToList(outMesh->indices, vertexIndex);
                    }
                    else {
                        vertexIndex = blxGetListCount(outMesh->vertices) - 1;
                        blxAddToHashTableAlloc(table, triangleP3, vertexIndex);
                        blxAddValueToList(outMesh->indices, vertexIndex);
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

                    unsigned int vertexIndex = blxGetListCount(outMesh->vertices);

                    //First Triangle ---------------
                    blxAddValueToList(outMesh->vertices, quadP1);
                    blxAddValueToList(outMesh->indices, vertexIndex);

                    vertexIndex++;
                    blxAddValueToList(outMesh->vertices, quadP2);
                    blxAddValueToList(outMesh->indices, vertexIndex);

                    vertexIndex++;
                    blxAddValueToList(outMesh->vertices, quadP3);
                    blxAddValueToList(outMesh->indices, vertexIndex);
                    //-----------

                    //Second Triangle -------------
                    //blxAddValueToList(outMesh->vertices, quadP1);
                    blxAddValueToList(outMesh->indices, vertexIndex - 2);

                    //blxAddValueToList(outMesh->vertices, quadP3);
                    blxAddValueToList(outMesh->indices, vertexIndex);
                    vertexIndex++;
                    blxAddValueToList(outMesh->vertices, quadP4);
                    blxAddValueToList(outMesh->indices, vertexIndex);
                   // vertexIndex++;
                   // blxAddValueToList(outMesh->indices, vertexIndex);
                   //-----------
                }

            } break;


            case 'u': {
                
                if (lineLength > 6)
                {
                    const char* firstWord;
                    blxStrnCpy(firstWord, lineBuffer, 6);
                    if(blxStrCmp(firstWord, "usemtl")){
                        //Grab material here!
                        //Assuming mat name is less than 64..
                        char matName[65];
                        blxStrCpy(matName, lineBuffer[7]);
                        //TODO: Assign material name!
                        blxMaterial* mat = blxMaterial_CreateDefault();
                        
                        break;
                    }
                }
                
        
            }break;
        }
    }

    blxFreeHashTable(table);
}

void blxImportModelFromObj(blxModel* outModel, const char* objPath)
{
    blxInitModel(outModel);
    //ExtractMesh()
}