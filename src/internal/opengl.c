#include "opengl.h"
#include "GL/glew.h"
#include "blx_rendering.h"
//TODO: Add support for index buffers


typedef struct
{
    unsigned int VAO, VBO, IBO;
}glMeshData;


void OpenGLInit()
{
    if (glewInit() != GLEW_OK) {
        printf("GLEW Failed to initialize\n");
        //return -1; cant return values, maybe try exit();
    }

}

void OpenGLDraw(blxMesh* mesh)
{
    glMeshData* md = (glMeshData*)mesh->_meshData;
    glUseProgram(mesh->shader);
    glBindVertexArray(md->VAO);
    glDrawElements(GL_TRIANGLES, blxGetListCount(mesh->indices), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void GlMeshTest(blxMesh* mesh) {
    printf("%d\n", ((glMeshData*)mesh->_meshData)->VAO);
    printf("%d\n", mesh->shader);
}

void OpenGLInitMesh(blxMesh* mesh) {
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
    //GlMeshTest(mesh);
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




