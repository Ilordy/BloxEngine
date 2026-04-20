#pragma once
#include "rendering/RenderCommands.h"

namespace BlxRend
{
    class IGraphicsDevice
    {
    public:
        virtual ~IGraphicsDevice() = 0;
        virtual void ExecuteCmdDrawMesh(CmdDrawMesh) = 0;
        virtual void ExecuteCmdSetSkybox(CmdSetSkybox) = 0;
    };
} // namespace BlxRend