#pragma once
#include "core/blx_defines.h"
#include "GL/glew.h"
#include "rendering/blx_texture.h"
#include "cglm/cglm.h"
#include "rendering/blx_renderingTypes.h"

//TODO: Remove useShader parameter.
BLXAPI Shader blxShaderCreate(const char* fragPath, const char* vertPath, GLboolean useShader);
BLXAPI void blxShaderSetBool(Shader shader, const char* name, GLboolean value);
BLXAPI void blxShaderSetInt(Shader shader, const char* name, GLint value);
BLXAPI void blxShaderSetFloat(Shader shader, const char* name, GLfloat value);
BLXAPI void blxShaderSetVec4(Shader shader, const char* name, vec4 value);
BLXAPI void blxShaderSetMatrix4f(Shader shader, const char* name, mat4 mat);
BLXAPI void blxShaderUseShader(Shader shader);
BLXAPI void blxShaderUseTexture(Shader shader, Texture* texture, const char* samplerName);
BLXAPI void blxShaderSetVec3(Shader shader, const char* name, vec3 value);

void _blxShaderSystemInitialize(GraphicsAPI graphicApi);
