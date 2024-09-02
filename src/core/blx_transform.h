#pragma once
#include "core/blx_defines.h"
#include <cglm/cglm.h>

typedef versor Quaternion;
typedef struct {
  vec3 position;
  Quaternion rotation;
  vec3 scale;
} Transform;

BLXAPI void transform_init(Transform* trans);
BLXAPI void transform_forward(Transform* trans, vec3 outVec);
BLXAPI void transform_up(Transform* trans, vec3 outVec);
BLXAPI void transform_right(Transform* trans, vec3 outVec);
BLXAPI void transform_eulerAngles(Transform* trans, vec3 outEulers);
BLXAPI void transform_setRotationEuler(Transform* trans, vec3 eulers);
BLXAPI void _transform_modelMatrix(Transform* trans, mat4 outModel);