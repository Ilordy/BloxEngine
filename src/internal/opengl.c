#include "opengl.h"
#include "GL/glew.h"
#include "rendering/blx_rendering.h"
#include "core/blx_logger.h"
//TODO: Add support for index buffers


typedef struct
{
    unsigned int VAO, VBO, IBO;
}glMeshData;


void OpenGLInit()
{
    glEnable(GL_DEPTH_TEST);
    GLint versionMajor;
    glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
    GLint versionMinor;
    glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
    BLXINFO("OpenGL Initialized using GL Version: %d.%d", versionMajor, versionMinor);
}

void OpenGLDraw(blxRenderPacket* packet)
{
    glClearColor(0.322f, 0.322f, 0.332f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (unsigned int i = 0; i < packet->modelCount; i++)
    {
        blxMesh mesh = packet->models[i].mesh;
        glMeshData* md = (glMeshData*)mesh._meshData;
        glUseProgram(mesh.shader);
        glBindVertexArray(md->VAO);
        shader_setMatrix4f(mesh.shader, "projection", packet->cam->projecionMatrix);
        //for specular lighting, possibly temp for now.
        shader_setVec3(mesh.shader, "camPos", packet->cam->transform.position);
        mat4 modelMatrix;
        _transform_modelMatrix(&packet->models[i].transform, modelMatrix);
        shader_setMatrix4f(mesh.shader, "model", modelMatrix);
        shader_setMatrix4f(mesh.shader, "view", packet->cam->viewMatrix);
        glDrawElements(GL_TRIANGLES, blxGetListCount(mesh.indices), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    for (unsigned int i = 0; i < packet->uiCount; i++)
    {
        blxMesh mesh = packet->ui[i].mesh;
        int g = blxGetListCount(mesh.vertices);
        glMeshData* md = (glMeshData*)mesh._meshData;
        glUseProgram(mesh.shader);
        glBindVertexArray(md->VAO);
        mat4 proj;
        _blxGetCameraProjection(&packet->cam, ORTHOGRAPHIC, proj);
        shader_setMatrix4f(mesh.shader, "projection", proj);
        mat4 modelMatrix;
        _transform_modelMatrix(&packet->ui[i].transform, modelMatrix);
        shader_setMatrix4f(mesh.shader, "model", modelMatrix);
        shader_setMatrix4f(mesh.shader, "view", GLM_MAT4_IDENTITY);
        glDrawElements(GL_TRIANGLES, blxGetListCount(mesh.indices), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void OpenGLInitMesh(blxMesh* mesh) {
    // if (mesh->_meshData != NULL) {
    //     // TODO: refactor this.
    //     printf("MESH DATA ALREADY INITIALIZED!\n");
    //     return; //MeshData already initialized.
    // }

    glMeshData* md = (glMeshData*)malloc(sizeof(glMeshData));
    glGenVertexArrays(1, &md->VAO);
    glBindVertexArray(md->VAO);
    glGenBuffers(1, &md->VBO);
    glGenBuffers(1, &md->IBO);
    glBindBuffer(GL_ARRAY_BUFFER, md->VBO);
    //Vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(blxVertex), (void*)0);
    glEnableVertexAttribArray(0);
    //Vertex normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(blxVertex), (void*)offsetof(blxVertex, normal));
    glEnableVertexAttribArray(1);
    //Vertex UVs
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(blxVertex), (void*)offsetof(blxVertex, uv));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    mesh->_meshData = (void*)md;
}

void OpenGLUpdateMesh(blxMesh* mesh)
{
    glMeshData* md = (glMeshData*)mesh->_meshData;
    glBindVertexArray(md->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, md->VBO);
    glBufferData(GL_ARRAY_BUFFER, blxGetListCount(mesh->vertices) * sizeof(blxVertex), mesh->vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, md->IBO);
    int g = blxGetListCount(mesh->indices);
    for (size_t i = 0; i < g; i++)
    {
        printf("%u\n", mesh->indices[i]);
    }

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, blxGetListCount(mesh->indices) * sizeof(unsigned int), mesh->indices, GL_STATIC_DRAW);
    glBindVertexArray(0);
}

void OpenGLSetShadingMode(blxShadingMode mode)
{
    switch (mode)
    {
        case BLX_SHADING_SOLID:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
        case BLX_SHADING_WIREFRAME:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
    }
}




