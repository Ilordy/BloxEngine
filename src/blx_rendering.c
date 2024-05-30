#include "blx_rendering.h"
#include "GL/glew.h"
#include <malloc.h>
#include "internal/opengl.h"
//TODO: List of meshes to render in the renderer.

typedef struct
{
    void (*Init)();
    void (*Draw)(blxMesh*);
    void (*UpdateMesh)(blxMesh*);
    blxMesh* (*CreateMesh)();
} blxRenderer;

blxRenderer* renderer;

void blxInitRenderer(GraphicsAPI graphicsToUse)
{
    if (!renderer) {
        renderer = (blxRenderer*)malloc(sizeof(blxRenderer));
    }
    switch (graphicsToUse)
    {
    case OPENGL:
        renderer->Draw = OpenGLDraw;
        renderer->Init = OpenGLInit;
        renderer->CreateMesh = OpenGLCreateMesh;
        renderer->UpdateMesh = OpenGLUpdateMesh;
        break;
    }

    renderer->Init();
}

blxMesh* blxCreateMesh()
{
    return renderer->CreateMesh();
}

void blxDraw(blxMesh* mesh)
{
    renderer->Draw(mesh);
}

void blxUpdateMesh(blxMesh* mesh)
{
    renderer->UpdateMesh(mesh);
}



