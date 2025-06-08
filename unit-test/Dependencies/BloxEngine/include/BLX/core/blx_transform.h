#pragma once
#include "core/blx_defines.h"
#include <cglm/cglm.h>

typedef versor Quaternion;

/// @brief Represents a transform in 3D space.
typedef struct blxTransform {
  /// @brief Position of the transform.
  vec3 position;
  /// @brief Rotation of the transform.
  Quaternion rotation;
  /// @brief Scale of the transform.
  vec3 scale;
} blxTransform;

/// @brief Initializes a transform.
/// @param trans The transform to initialize.
BLXAPI void blxTransform_Init(blxTransform* trans);

/// @brief Calculates the forward direction vector of the transform.
/// @param trans The transform to calculate from.
/// @param outVec The resulting forward direction vector.
BLXAPI void blxTransform_Forward(blxTransform* trans, vec3 outVec);

/// @brief Calculates the up direction vector of the transform.
/// @param trans The transform to calculate from.
/// @param outVec The resulting up direction vector.
BLXAPI void blxTransform_Up(blxTransform* trans, vec3 outVec);

/// @brief Calculates the right direction vector of the transform.
/// @param trans The transform to calculate from.
/// @param outVec The resulting right direction vector.
BLXAPI void blxTransform_Right(blxTransform* trans, vec3 outVec);

/// @brief Retrieves the Euler angles (pitch, yaw, roll) of the transform's rotation.
/// @param trans The transform to retrieve from.
/// @param outEulers The resulting Euler angles.
BLXAPI void blxTransform_EulerAngles(blxTransform* trans, vec3 outEulers);

/// @brief Sets the transform's rotation using Euler angles (pitch, yaw, roll).
/// @param trans The transform to modify.
/// @param eulers The Euler angles to set.
BLXAPI void blxTransform_SetRotationEuler(blxTransform* trans, vec3 eulers);

/// @brief Calculates the model matrix for the transform. For internal use only.
/// @param trans The transform to calculate from.
/// @param outModel The resulting model matrix.
BLXAPI void _transform_modelMatrix(blxTransform* trans, mat4 outModel);