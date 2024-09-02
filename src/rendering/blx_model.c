#include "blx_model.h"

void blxInitModel(blxModel* model)
{
    //blxInitMesh(&model->mesh);
    transform_init(&model->transform);
}

void blxSetModelMesh(blxModel* model, blxMesh* mesh)
{
    //TODO: Gotta make it so we can copy mesh vertices.
}
