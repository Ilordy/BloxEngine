#pragma once
#include "rendering/blx_rendering.h"
#include "GL/glew.h"
#include "core/blx_defines.h"
//TODO: Refactor function names.

void blxGL_Draw(blxRenderPacket* packet);
void blxGL_Init();
void blxGL_InitMesh(blxMesh* mesh);
// TODO: Verify this function is no longer needed.
void OpenGLUpdateMesh(blxMesh* mesh);
void blxGL_SetShadingMode(blxShadingMode mode);
void blxGL_SetFloat(GLuint shader, const char* uniformName, GLfloat value);
void blxGL_SetInt(GLuint shader, const char* uniformName, GLint value);
void blxGL_SetVec4f(GLuint shader, const char* uniformName, vec4 value);
void blxGL_SetBool(GLuint shader, const char* uniformName, GLboolean value);
void blxGL_SetVec3f(GLuint shader, const char* uniformName, vec3 value);
void blxGL_SetMatrix4f(GLuint shader, const char* uniformName, mat4 value);
void blxGL_RegisterBatch(MaterialGroup* matGroup);
GLuint blxGL_CreateShader(const char* fragSource, const char* vertSource);