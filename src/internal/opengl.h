#pragma once
#include "blx_rendering.h"
#include "Defines.h"

void OpenGLDraw(blxRenderPacket* packet);
void OpenGLInit();
void OpenGLInitMesh(blxMesh* mesh);
void OpenGLUpdateMesh(blxMesh* mesh);