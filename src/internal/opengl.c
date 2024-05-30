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

}

void GlMeshTest(blxMesh* mesh) {
    printf("%d\n", ((glMeshData*)mesh->_meshData)->VAO);
    printf("%d\n", mesh->shader);
}

blxMesh* OpenGLCreateMesh() {
    blxMesh* mesh = malloc(sizeof(blxMesh));
    glMeshData* md = (glMeshData*)malloc(sizeof(glMeshData));
    glGenVertexArrays(1, &md->VAO);
    glBindVertexArray(md->VAO);
    glGenBuffers(1, &md->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, md->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mesh->vertices), mesh->vertices, GL_STATIC_DRAW);
    // 	unsigned int VAO;
  // 	glGenVertexArrays(1, &VAO);
  // 	glBindVertexArray(VAO);
    mesh->shader = 12;
    GlMeshTest(mesh);
    mesh->_meshData = (void*)md;
    return mesh;
}

void OpenGLUpdateMesh(blxMesh* mesh)
{
    glMeshData* md = (glMeshData*)mesh->_meshData;
    glBindVertexArray(md->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, md->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mesh->vertices), mesh->vertices, GL_STATIC_DRAW);
}




