#include "blx_model.h"

void blxInitModel(blxModel* model)
{
    transform_init(&model->transform);
    model->geometries = blxInitList(blxRenderableGeometry);
}

void blxSetModelMesh(blxModel* model, blxMesh* mesh)
{
    //TODO: Gotta make it so we can copy mesh vertices.
}
