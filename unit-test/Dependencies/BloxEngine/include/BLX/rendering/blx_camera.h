#pragma once
#include "core/blx_transform.h"
#include <cglm/cam.h>
#include <cglm/cglm.h>

typedef enum { PERSPECTIVE = 0, ORTHOGRAPHIC = 1 } CameraProjectionType;

typedef struct {
  float fov;
  blxTransform transform;
  CameraProjectionType projectionType;
  mat4 projecionMatrix, viewMatrix;
} Camera;

void _blxGetCameraProjection(Camera* cam, CameraProjectionType projType, mat4 dest);

/// @brief Initializes a camera with a given field of view.
/// @param cam The camera to initialize.
/// @param fov The field of view for the camera.
BLXAPI void blxCamera_Init(Camera* cam, float fov);

/// @brief Sets the projection type for a given camera.
/// @param cam The camera to set the projection type for.
/// @param projType The projection type to set.
BLXAPI void blxCamera_SetProjection(Camera* cam, CameraProjectionType projType);

// TODO: This should go into an internal file.
BLXAPI void camera_update(Camera* cam);
