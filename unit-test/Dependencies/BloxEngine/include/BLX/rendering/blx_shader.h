#pragma once
#include "core/blx_defines.h"
#include "GL/glew.h"
#include "rendering/blx_texture.h"
#include "cglm/cglm.h"
#include "rendering/blx_renderingTypes.h"

// TODO: User should not have access to these functions.

//TODO: Remove useShader parameter.
BLXAPI Shader blxShader_Create(const char* fragPath, const char* vertPath, GLboolean useShader);
BLXAPI void blxShader_SetBool(Shader shader, const char* name, GLboolean value);
BLXAPI void blxShader_SetInt(Shader shader, const char* name, GLint value);
BLXAPI void blxShader_SetFloat(Shader shader, const char* name, GLfloat value);
BLXAPI void blxShader_SetVec4(Shader shader, const char* name, vec4 value);
BLXAPI void blxShader_SetMatrix4f(Shader shader, const char* name, mat4 mat);
BLXAPI void blxShader_UseShader(Shader shader);
BLXAPI void blxShader_UseTexture(Shader shader, Texture* texture, const char* samplerName);
BLXAPI void blxShader_SetVec3(Shader shader, const char* name, vec3 value);

void _blxShader_SystemInitialize(GraphicsAPI graphicApi);
