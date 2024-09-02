#pragma once
#include "core/blx_defines.h"
#include "GL/glew.h"
#include "rendering/blx_texture.h"
#include "cglm/cglm.h"


typedef GLuint Shader;

BLXAPI Shader shader_create(const char* fragPath, const char* vertPath, GLboolean useShader);
BLXAPI void shader_setBool(Shader shader, const char* name, GLboolean value);
BLXAPI void shader_setInt(Shader shader, const char* name, GLint value);
BLXAPI void shader_setFloat(Shader shader, const char* name, GLfloat value);
BLXAPI void shader_setFloat4f(Shader shader, const char* name, vec4 value);
BLXAPI void shader_setMatrix4f(Shader shader, const char* name, mat4 mat);
BLXAPI void shader_useShader(Shader shader);
BLXAPI void shader_useTexture(Shader shader, Texture* texture, const char* samplerName);
BLXAPI void shader_setVec3(Shader shader, const char* name, vec3 value);
