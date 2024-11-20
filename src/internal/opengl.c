#include "opengl.h"
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
        blxShaderSetMatrix4f(mesh.shader, "projection", packet->cam->projecionMatrix);
        //for specular lighting, possibly temp for now.
        blxShaderSetVec3(mesh.shader, "camPos", packet->cam->transform.position);
        mat4 modelMatrix;
        _transform_modelMatrix(&packet->models[i].transform, modelMatrix);
        blxShaderSetMatrix4f(mesh.shader, "model", modelMatrix);
        blxShaderSetMatrix4f(mesh.shader, "view", packet->cam->viewMatrix);
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
        blxShaderSetMatrix4f(mesh.shader, "projection", proj);
        mat4 modelMatrix;
        _transform_modelMatrix(&packet->ui[i].transform, modelMatrix);
        blxShaderSetMatrix4f(mesh.shader, "model", modelMatrix);
        blxShaderSetMatrix4f(mesh.shader, "view", GLM_MAT4_IDENTITY);
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

#pragma region Shaders
static blxBool blxGLCheckShaderError(GLuint shader, GLenum errorType)
{
    //DELETE THIS FUNC IF ONLY USE IS FOR GL_COMPILE_STATUS....
    blxBool success;
    char log[512];
    glGetShaderiv(shader, errorType, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, log);
        printf("[SHADER ERROR] %s", log);
        glGetShaderSource(shader, 512, NULL, log);
        printf("%s", log);
    }
    return !success;
}

GLuint blxGLCreateShader(const char* fragSource, const char* vertSource)
{
    char errorLog[512];
    blxBool compiled;

    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertSource, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        glGetShaderInfoLog(vertexShader, sizeof(errorLog), NULL, errorLog);
        BLXERROR("%s\n Source Code:\n %s", errorLog, vertSource);
        return -1;
    }

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragSource, NULL);
    glCompileShader(fragmentShader);
    if (!compiled)
    {
        glGetShaderInfoLog(fragmentShader, sizeof(errorLog), NULL, errorLog);
        BLXERROR("%s\n Source Code:\n %s", errorLog, fragSource);
        return -1;
    }

    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // TODO: ERROR IS HERE ON LINE 91, PLS FIX!
    //Use glGetProgramiv instead of shaderiv.
    // if (shader_checkError(shaderProgram, GL_LINK_STATUS))
    // {
    // 	return -1;
    // }

    // if (useShader)
    // {
    //     glUseProgram(shaderProgram);
    // }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}


static GLint blxGLGetUniform(GLuint shader, const char* name)
{
    //TODO: Figure out if this is temporary...
    GLint loc = glGetUniformLocation(shader, name);
    if (loc == -1)
    {
        BLXWARNING("Shader uniform %s not found!", name);
    }
    return loc;
}

void blxGLSetFloat(GLuint shader, const char* uniformName, GLfloat value)
{
    glUniform1f(blxGLGetUniform(shader, uniformName), value);
}

void blxGLSetInt(GLuint shader, const char* uniformName, GLint value)
{
    glUniform1i(blxGLGetUniform(shader, uniformName), value);
}

void blxGLSetVec4f(GLuint shader, const char* uniformName, vec4 value)
{
    glUniform4f(blxGLGetUniform(shader, uniformName), value[0], value[1], value[2], value[3]);
}

void blxGLSetBool(GLuint shader, const char* uniformName, GLboolean value)
{
    glUniform1i(blxGLGetUniform(shader, uniformName), value);
}

void blxGLSetVec3f(GLuint shader, const char* uniformName, vec3 value)
{
    glUniform3f(blxGLGetUniform(shader, uniformName), value[0], value[1], value[2]);
}

void blxGLSetMatrix4f(GLuint shader, const char* uniformName, mat4 value)
{
    glUniformMatrix4fv(blxGLGetUniform(shader, uniformName), 1, GL_FALSE, value);
}
#pragma endregion




