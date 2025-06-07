#include "opengl.h"
#include "rendering/blx_rendering.h"
#include "core/blx_logger.h"
#include "core/blx_memory.h"
#include "utils/blx_vlist.h"
//TODO: Add support for index buffers

typedef struct {
    mat4 model;
    mat4 view;
    mat4 projection;
}transformMatrices;

typedef struct
{
    mat4* modelMatrices;
    GLuint VAO, VBO, IBO, cmdBuffer, ssbo;
}glMeshData;


typedef struct {
    /// @brief Number of indices.
    GLuint  count;
    /// @brief Number of instances to draw.
    GLuint  instanceCount;
    /// @brief Index offset in the indices array in which to start reading.
    GLuint  firstIndex;
    /// @brief Index offset in the vertices array in which to start reading.
    GLuint  baseVertex;
    /// @brief Any additioal data we want to pass to the GPU (gl_instanceID)
    GLuint  baseInstance;
}glDrawCommand;

typedef glDrawCommand* vlist_glDrawCommand;

static vlist_glDrawCommand drawCommands;
static GLuint glIndirectBuffer;
//static glDrawCommand* commands;

void blxGL_Init()
{
    glEnable(GL_DEPTH_TEST);
    //TODO: This should only be enabled in a debug context of opengl.
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    GLint versionMajor;
    glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
    GLint versionMinor;
    glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
    drawCommands = blxInitList(glDrawCommand);
    BLXINFO("OpenGL Initialized using GL Version: %d.%d", versionMajor, versionMinor);
}

void blxGL_RegisterBatch(MaterialGroup* matGroup)
{
    glMeshData* data = blxAllocate(sizeof(glMeshData), BLXMEMORY_TAG_RENDERER);
    glGenVertexArrays(1, &data->VAO);
    glBindVertexArray(data->VAO);
    glGenBuffers(1, &data->cmdBuffer);
    glGenBuffers(1, &data->VBO);
    glGenBuffers(1, &data->IBO);
    glGenBuffers(1, &data->ssbo);
    glBindBuffer(GL_ARRAY_BUFFER, data->VBO);
    data->modelMatrices = blxInitList(mat4);

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
    matGroup->renderData = data;
}

//TODO: Generate draw commands on a different thread.

static void GenerateDrawCommands(struct MaterialGroup* batch)
{
    unsigned int numMeshes = blxGetListCount(batch->vlist_meshes);
    unsigned int vertOffset = 0;
    unsigned int indexOffset = 0;

    for (uint64 i = 0; i < numMeshes; i++)
    {
        glDrawCommand command;
        unsigned int numIndicies = blxGetListCount(batch->vlist_meshes[i].indices);
        command.instanceCount = 1;
        command.count = numIndicies;
        command.firstIndex = indexOffset;
        command.baseVertex = vertOffset;
        command.baseInstance = i;
        vertOffset += blxGetListCount(batch->vlist_meshes[i].vertices);
        indexOffset += numIndicies;
        blxAddValueToList(drawCommands, command);
    }
}

// TODO: It should be one draw call per shader not one draw call per material..
void blxGL_Draw(blxRenderPacket* packet)
{
    glClearColor(0.322f, 0.322f, 0.332f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (uint64 i = 0; i < blxGetListCount(packet->vlist_materialGroups); i++)
    {
        MaterialGroup currentBatch = packet->vlist_materialGroups[i];
        glMeshData* batchData = (glMeshData*)currentBatch.renderData;

        //TODO: We should not be buffering data every frame.
        glBindVertexArray(batchData->VAO);
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, batchData->cmdBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchData->IBO);
        glBindBuffer(GL_ARRAY_BUFFER, batchData->VBO);

        //Clear the list from the last draw call.
        blxClearList(drawCommands);
        GenerateDrawCommands(&currentBatch);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * blxGetListCount(currentBatch.indices), currentBatch.indices, GL_DYNAMIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, sizeof(blxVertex) * blxGetListCount(currentBatch.vertices), currentBatch.vertices, GL_DYNAMIC_DRAW);
        glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(glDrawCommand) * blxGetListCount(drawCommands), drawCommands, GL_DYNAMIC_DRAW);

        glUseProgram(currentBatch.material->shader);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, batchData->ssbo);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, batchData->ssbo);

        // compute model matrices for each geometry in the batch.
        for (uint64 i = 0; i < blxGetListCount(currentBatch.geometries); i++)
        {
            mat4 model;
            _transform_modelMatrix(currentBatch.geometries[i].transform, model);

            blxAddBufferToList(batchData->modelMatrices, model);
        }

        blxShader_SetMatrix4f(currentBatch.material->shader, "projection", packet->cam->projecionMatrix);
        blxShader_SetMatrix4f(currentBatch.material->shader, "view", packet->cam->viewMatrix);

        //Buffer transform data here.
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(mat4) * blxGetListCount(batchData->modelMatrices), batchData->modelMatrices, GL_DYNAMIC_DRAW);
        
        _blxMaterialSetValues(currentBatch.material);

        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, (const void*)0, blxGetListCount(drawCommands), 0);

        blxClearList(batchData->modelMatrices);

    }

    //glMultiDrawElementsIndirect()
    //glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, )
        // for (unsigned int i = 0; i < packet->modelCount; i++)
        // {
        //     blxModel model = packet->models[i];
        //     for (unsigned int j = 0; j < model.mCount; i++)
        //     {
        //         blxMesh* m = model.meshes[j];
        //     }

        //     blxMesh mesh = packet->models[i];
        //     glMeshData* md = (glMeshData*)mesh._meshData;
        //     glUseProgram(mesh.shader);
        //     glBindVertexArray(md->VAO);
        //     blxShaderSetMatrix4f(mesh.shader, "projection", packet->cam->projecionMatrix);
        //     //for specular lighting, possibly temp for now.
        //     blxShaderSetVec3(mesh.shader, "camPos", packet->cam->transform.position);
        //     mat4 modelMatrix;
        //     _transform_modelMatrix(&packet->models[i].transform, modelMatrix);
        //     blxShaderSetMatrix4f(mesh.shader, "model", modelMatrix);
        //     blxShaderSetMatrix4f(mesh.shader, "view", packet->cam->viewMatrix);
        //     glDrawElements(GL_TRIANGLES, blxGetListCount(mesh.indices), GL_UNSIGNED_INT, 0);
        //     glBindVertexArray(0);
        // }

        // for (unsigned int i = 0; i < packet->uiCount; i++)
        // {
        //     blxMesh mesh = packet->ui[i].mesh;
        //     int g = blxGetListCount(mesh.vertices);
        //     glMeshData* md = (glMeshData*)mesh._meshData;
        //     glUseProgram(mesh.shader);
        //     glBindVertexArray(md->VAO);
        //     mat4 proj;
        //     _blxGetCameraProjection(&packet->cam, ORTHOGRAPHIC, proj);
        //     blxShaderSetMatrix4f(mesh.shader, "projection", proj);
        //     mat4 modelMatrix;
        //     _transform_modelMatrix(&packet->ui[i].transform, modelMatrix);
        //     blxShaderSetMatrix4f(mesh.shader, "model", modelMatrix);
        //     blxShaderSetMatrix4f(mesh.shader, "view", GLM_MAT4_IDENTITY);
        //     glDrawElements(GL_TRIANGLES, blxGetListCount(mesh.indices), GL_UNSIGNED_INT, 0);
        //     glBindVertexArray(0);
        // }
}

// TODO: OLD PLS DELETE THIS LATER
void blxGL_InitMesh(blxMesh* mesh) {
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

void blxGL_SetShadingMode(blxShadingMode mode)
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

GLuint blxGL_CreateShader(const char* fragSource, const char* vertSource)
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
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
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
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &compiled);

    if (!compiled)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(errorLog), NULL, errorLog);
        BLXERROR("%s", errorLog);
        return -1;
    }

    // if (useShader)
    // {
    //     glUseProgram(shaderProgram);
    // }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}


//TODO: Use asserts instead of using glgetuniform, or if statements.
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

void blxGL_SetFloat(GLuint shader, const char* uniformName, GLfloat value)
{
    glProgramUniform1f(shader, blxGLGetUniform(shader, uniformName), value);
}

void blxGL_SetInt(GLuint shader, const char* uniformName, GLint value)
{
    glProgramUniform1i(shader, blxGLGetUniform(shader, uniformName), value);
}

void blxGL_SetVec4f(GLuint shader, const char* uniformName, vec4 value)
{
    glProgramUniform4f(shader, blxGLGetUniform(shader, uniformName), value[0], value[1], value[2], value[3]);
}

void blxGL_SetBool(GLuint shader, const char* uniformName, GLboolean value)
{
    glProgramUniform1i(shader, blxGLGetUniform(shader, uniformName), value);
}

void blxGL_SetVec3f(GLuint shader, const char* uniformName, vec3 value)
{
    glProgramUniform3f(shader, blxGLGetUniform(shader, uniformName), value[0], value[1], value[2]);
}

void blxGL_SetMatrix4f(GLuint shader, const char* uniformName, mat4 value)
{
    glProgramUniformMatrix4fv(shader, blxGLGetUniform(shader, uniformName), 1, GL_FALSE, value);
}
#pragma endregion




