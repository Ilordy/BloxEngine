#include "blx_model.h"
#include "utils/blx_vlist.h"

void blxModel_Init(blxModel* model)
{
    blxTransform_Init(&model->transform);
    model->geometries = blxInitList(blxRenderableGeometry);
}

void blxSetModelMesh(blxModel* model, blxMesh* mesh)
{
    //TODO: Gotta make it so we can copy mesh vertices.
}
