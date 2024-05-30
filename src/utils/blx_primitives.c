#include "blx_rendering.h"
#include "blx_primitives.h"
//TODO: ADD SUPPORT FOR NORMALS

// float cubeData[] = {
//     //vertex positions                       //UV Cords
//     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
//      0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
//      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//     -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

//     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//      0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
//     -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
//     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

//     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//     -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//     -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

//      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//      0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//      0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

//     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
//      0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
//      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//      0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
//     -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
//     -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

//     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
//      0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
//      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//      0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
//     -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
//     -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
// };



// float t2[] = {
//     //pos                 //normals          //uv
//      -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
//     1.0f, -1.0f,  1.0f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
//     -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
//     1.0f,  1.0f,  1.0f,   1.0f,  0.0f, 0.0f, 1.0f, 1.0f,
//     -1.0f, -1.0f, -1.0f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
//     1.0f, -1.0f, -1.0f,   1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
//     -1.0f,  1.0f, -1.0f,  -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
//     1.0f,  1.0f, -1.0f,   0.0f,  0.0f, -1.0f,  1.0f, 1.0f
// };

blxModel* MakeCube() {
    blxMesh mesh;
    blxModel* model = (blxModel*)malloc(sizeof(blxModel));
    //8 verts for a cube
    mesh.vertices = blxInitListWithSize(vList_blxVertex, 8);
    mesh.vertices[0] = (blxVertex){ -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
    mesh.vertices[1] = (blxVertex){ 1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f };
    mesh.vertices[2] = (blxVertex){ -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f };
    mesh.vertices[3] = (blxVertex){ -1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f };
    mesh.vertices[4] = (blxVertex){ -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f };
    mesh.vertices[5] = (blxVertex){ 1.0f, -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
    mesh.vertices[6] = (blxVertex){ -1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    mesh.vertices[7] = (blxVertex){ 1.0f,  1.0f, -1.0f,  0.0f, 0.0f, -1.0f, 1.0f, 1.0f };
    //36 indices -> 12 triangles or faces
    mesh.indices = blxInitListWithSize(vList_indices, 36);
    mesh.indices[0] = 2;
    mesh.indices[1] = 6;
    mesh.indices[2] = 7;
    mesh.indices[3] = 2;
    mesh.indices[4] = 3;
    mesh.indices[5] = 7;
    mesh.indices[6] = 0;
    mesh.indices[7] = 4;
    mesh.indices[8] = 5;
    mesh.indices[9] = 0;
    mesh.indices[10] = 1;
    mesh.indices[11] = 5;
    mesh.indices[12] = 0;
    mesh.indices[13] = 2;
    mesh.indices[14] = 6;
    mesh.indices[15] = 0;
    mesh.indices[16] = 4;
    mesh.indices[17] = 6;
    mesh.indices[18] = 1;
    mesh.indices[19] = 3;
    mesh.indices[20] = 7;
    mesh.indices[21] = 1;
    mesh.indices[22] = 5;
    mesh.indices[23] = 7;
    mesh.indices[24] = 0;
    mesh.indices[25] = 2;
    mesh.indices[26] = 3;
    mesh.indices[27] = 0;
    mesh.indices[28] = 1;
    mesh.indices[29] = 3;
    mesh.indices[30] = 4;
    mesh.indices[31] = 6;
    mesh.indices[32] = 7;
    mesh.indices[33] = 4;
    mesh.indices[34] = 5;
    mesh.indices[35] = 7;
    transform_init(&model->transform);
    model->mesh = mesh;
    return model;
}