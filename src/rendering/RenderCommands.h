#pragma once
#include "core/blx_defines.h"
#include "rendering/blx_model.h"

BLXEXTERN_C_START
#include "blx_renderingTypes.h"
BLXEXTERN_C_END

namespace BlxRendering
{
    struct CmdDrawMesh
    {
        blxRenderableGeometry geometry;
        blxMaterial material;
    };

    struct CmdDrawModel
    {
        blxModel* model;
    };

    struct CmdSetSkybox
    {
        uint32_t skyboxId;
    };
} // namespace BlxRendering