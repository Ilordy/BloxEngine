#pragma once
#include "blx_rendering.h"
#include "Defines.h"

void OpenGLDraw(blxMesh* mesh);
void OpenGLInit();
blxMesh* OpenGLCreateMesh();
void OpenGLUpdateMesh(blxMesh* mesh);