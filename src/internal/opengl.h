#pragma once
#include "rendering/blx_rendering.h"
#include "core/blx_defines.h"

void OpenGLDraw(blxRenderPacket* packet);
void OpenGLInit();
void OpenGLInitMesh(blxMesh* mesh);
void OpenGLUpdateMesh(blxMesh* mesh);
void OpenGLSetShadingMode(blxShadingMode mode);