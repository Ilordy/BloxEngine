#include "blx_imGui.h"
#include "rendering/blx_rendering.h"

// //Quad data for UI

// float vertices[] = {
//      0.5f,  0.5f, 0.0f,  // top right
//      0.5f, -0.5f, 0.0f,  // bottom right
//     -0.5f, -0.5f, 0.0f,  // bottom left
//     -0.5f,  0.5f, 0.0f   // top left 
// };
// unsigned int indices[] = {
//     0, 1, 3,   // first triangle
//     1, 2, 3    // second triangle
// };


// static blxMesh defaultUIGeometry;
// static blxModel* btn;

// blxBool blxImGuiBtn()
// {
    
//     blxInitModel(btn);
//     btn->mesh = defaultUIGeometry;
//     int h = blxGetListCount(btn->mesh.vertices);
//     blxDrawUI(btn);
//     return BLX_FALSE;
// }

// void blxImGuiInit()
// {
//     btn = malloc(sizeof(blxModel));
//     blxInitMesh(&defaultUIGeometry);
//   //TODO: Make these vertices and indices part of a built in obj file.
//     defaultUIGeometry.vertices = blxInitListWithSize(vList_blxVertex, 4);
//     defaultUIGeometry.indices = blxInitListWithSize(vList_indices, 6);

//     blxAddValueToList(defaultUIGeometry.vertices, ((blxVertex)
//         //position
//     {   0.5f, 0.5f, 0.0f,
//         //normal
//         0.0f, 0.0f, 0.0f,
//         //uv
//         1.0f, 1.0f
//     }));
//     blxAddValueToList(defaultUIGeometry.vertices, ((blxVertex)
//         //position
//     {   0.5f, -0.5f, 0.0f,
//         //normal
//         0.0f, 0.0f, 0.0f,
//         //uv
//         1.0f, 0.0f
//     }));
//     blxAddValueToList(defaultUIGeometry.vertices, ((blxVertex)
//         //position
//     {   -0.5f, -0.5f, 0.0f,
//         //normal
//         0.0f, 0.0f, 0.0f,
//         //uv
//         0.0f, 0.0f
//     }));
//     blxAddValueToList(defaultUIGeometry.vertices, ((blxVertex)
//         //position
//     {   -0.5f, 0.5f, 0.0f,
//         //normal
//         0.0f, 0.0f, 0.0f,
//         //uv
//         0.0f, 1.0f
//     }));

//     blxAddValueToList(defaultUIGeometry.indices, 0);
//     blxAddValueToList(defaultUIGeometry.indices, 1);
//     blxAddValueToList(defaultUIGeometry.indices, 3);
//     blxAddValueToList(defaultUIGeometry.indices, 1);
//     blxAddValueToList(defaultUIGeometry.indices, 2);
//     blxAddValueToList(defaultUIGeometry.indices, 3);
//     //End TODO
//     blxUpdateMesh(&defaultUIGeometry);
// }
