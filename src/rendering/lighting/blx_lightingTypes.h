#pragma once
#include "cglm/struct/vec3.h"
#include "core/blx_transform.h"

typedef struct {
    vec3s direction;
    vec3s diffuse;
}blxDirectionalLight;

typedef struct {
    Transform transform;
    vec3s diffuse;
}blxPointLight;