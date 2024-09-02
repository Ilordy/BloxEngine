#include "rendering/blx_rendering.h"
#include "utils/blx_primitives.h"

blxMesh* blxCubeMesh() {
    blxMesh* mesh = (blxMesh*)malloc(sizeof(blxMesh));
    blxInitMesh(mesh);
    mesh->vertices = blxInitListWithSize(vList_blxVertex, 36);
    blxAddValueToList(mesh->vertices, ((blxVertex)
        //position
    {   -0.5f, -0.5f, -0.5f,
        //normal
        0.0f, 0.0f, -1.0f,
        //uv
        0.0f, 0.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {   0.5f, -0.5f, -0.5f,
        0.0f, 0.0f, -1.0f,
        1.0f, 0.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {   0.5f, 0.5f, -0.5f,
        0.0f, 0.0f, -1.0f,
        1.0f, 1.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {   0.5f, 0.5f, -0.5f,
        0.0f, 0.0f, -1.0f,
        1.0f, 1.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {  -0.5f, 0.5f, -0.5f,
        0.0f, 0.0f, -1.0f,
        0.0f, 1.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {  -0.5f, -0.5f, -0.5f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {  -0.5f, -0.5f, 0.5f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {   0.5f, -0.5f, 0.5f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {   0.5f, 0.5f, 0.5f,
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {   0.5f, 0.5f, 0.5f,
        0.0f, 0.0f, 1.0f,
        1.0f, 1.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {  -0.5f, 0.5f, 0.5f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {  -0.5f, -0.5f, 0.5f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {  -0.5f, 0.5f, 0.5f,
        -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {  -0.5f, 0.5f, -0.5f,
        -1.0f, 0.0f, 0.0f,
        1.0f, 0.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {  -0.5f, -0.5f, -0.5f,
        -1.0f, 0.0f, 0.0f,
        1.0f, 1.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {  -0.5f, -0.5f, -0.5f,
        -1.0f, 0.0f, 0.0f,
        1.0f, 1.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {  -0.5f, -0.5f, 0.5f,
        -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {  -0.5f, 0.5f, 0.5f,
        -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {   0.5f, 0.5f, 0.5f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {   0.5f, 0.5f, -0.5f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {   0.5f, -0.5f, -0.5f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {   0.5f, -0.5f, -0.5f,
        1.0f, 0.0f, 0.0f,
        1.0f, 1.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {   0.5f, -0.5f, 0.5f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {   0.5f, 0.5f, 0.5f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {  -0.5f, -0.5f, -0.5f,
        0.0f, -1.0f, 0.0f,
        0.0f, 0.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {   0.5f, -0.5f, -0.5f,
        0.0f, -1.0f, 0.0f,
        1.0f, 0.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {   0.5f, -0.5f, 0.5f,
        0.0f, -1.0f, 0.0f,
        1.0f, 1.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {   0.5f, -0.5f, 0.5f,
        0.0f, -1.0f, 0.0f,
        1.0f, 1.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {  -0.5f, -0.5f, 0.5f,
        0.0f, -1.0f, 0.0f,
        0.0f, 1.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {  -0.5f, -0.5f, -0.5f,
        0.0f, -1.0f, 0.0f,
        0.0f, 0.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {  -0.5f, 0.5f, -0.5f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {   0.5f, 0.5f, -0.5f,
        0.0f, 1.0f, 0.0f,
        1.0f, 0.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {   0.5f, 0.5f, 0.5f,
        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {   0.5f, 0.5f, 0.5f,
        0.0f, 1.0f, 0.0f,
        1.0f, 1.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {  -0.5f, 0.5f, 0.5f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f
    }));

    blxAddValueToList(mesh->vertices, ((blxVertex)
    {  -0.5f, 0.5f, -0.5f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f
    }));
    //36 indices -> 12 triangles or faces
    unsigned int* indices = (unsigned int*)malloc(sizeof(unsigned int) * 36);

    // TODO: Fix normals! Some indices are incorrect for the corresponding normal!
    // Front face
    indices[0] = 0;
    indices[1] = 1;
    indices[2] = 2;
    indices[3] = 4;
    indices[4] = 2;
    indices[5] = 0;

    // Back face
    indices[6] = 6;
    indices[7] = 7;
    indices[8] = 8;
    indices[9] = 10;
    indices[10] = 8;
    indices[11] = 6;

    // Left face
    indices[12] = 0;
    indices[13] = 6;
    indices[14] = 10;
    indices[15] = 10;
    indices[16] = 4;
    indices[17] = 0;

    // Right face
    indices[18] = 1;
    indices[19] = 7;
    indices[20] = 8;
    indices[21] = 8;
    indices[22] = 2;
    indices[23] = 1;

    // Bottom face
    indices[24] = 0;
    indices[25] = 1;
    indices[26] = 6;
    indices[27] = 6;
    indices[28] = 7;
    indices[29] = 1;

    // Top face
    indices[30] = 2;
    indices[31] = 8;
    indices[32] = 10;
    indices[33] = 10;
    indices[34] = 4;
    indices[35] = 2;
    mesh->indices = blxInitListWithSize(vList_indices, 36);
    blxAddArrayToList(mesh->indices, indices, 36);
    free(indices);
    blxUpdateMesh(mesh);
    return mesh;
}