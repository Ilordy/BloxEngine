#pragma once
#include "rendering/blx_rendering.h"
#include "GL/glew.h"
#include "core/blx_defines.h"
//TODO: Refactor function names.

void OpenGLDraw(blxRenderPacket* packet);
void OpenGLInit();
void OpenGLInitMesh(blxMesh* mesh);
void OpenGLUpdateMesh(blxMesh* mesh);
void OpenGLSetShadingMode(blxShadingMode mode);
void blxGLSetFloat(GLuint shader, const char* uniformName, GLfloat value);
void blxGLSetInt(GLuint shader, const char* uniformName, GLint value);
void blxGLSetVec4f(GLuint shader, const char* uniformName, vec4 value);
void blxGLSetBool(GLuint shader, const char* uniformName, GLboolean value);
void blxGLSetVec3f(GLuint shader, const char* uniformName, vec3 value);
void blxGLSetMatrix4f(GLuint shader, const char* uniformName, mat4 value);
GLuint blxGLCreateShader(const char* fragSource, const char* vertSource);