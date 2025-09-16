#include "core/blx_transform.h"
#include "core/blx_defines.h"
#include "blx_transform.h"

void blxTransform_Init(blxTransform* trans) {
  glm_vec3(GLM_VEC3_ZERO, trans->position);
  glm_vec3(GLM_VEC3_ONE, trans->scale);
  glm_quat_identity(trans->rotation);
}

void _transform_modelMatrix(blxTransform* trans, mat4 outModel) {
  mat4 t, r, s;
  glm_translate_make(t, trans->position);
  glm_quat_mat4(trans->rotation, r);
  glm_scale_make(s, trans->scale);
  mat4 tr;
  glm_mat4_mul(t, r, tr);
  glm_mat4_mul(s, tr, outModel);
}

vec3s blxTrasform_TransformPoint(const blxTransform* trans, const vec3s localPoint)
{
  mat4 model;
  _transform_modelMatrix(trans, model);

  vec3s worldPoint;
  // Multiply the local point by the model matrix to get the world point
  glm_mat4_mulv3(model, localPoint.raw, 1.0f, worldPoint.raw);

  return worldPoint;
}

vec3s blxTransform_InverseTransformPoint(const blxTransform* trans, const vec3s worldPoint)
{
  mat4 model;
  _transform_modelMatrix(trans, model);

  vec3s localPoint;
  glm_mat4_inv(model, model);

  // Multiply the world point by the inverse model matrix to get the local point
  glm_mat4_mulv3(model, worldPoint.raw, 1.0f, localPoint.raw);
  return localPoint;
}

vec3s blxTransform_TransformDirection(const blxTransform* trans, const vec3s direction)
{
  mat4 rot;
  glm_quat_mat4(trans->rotation, rot);

  vec3s worldDirection;
  // Multiply the direction by the rotation matrix to get the world direction
  glm_mat4_mulv3(rot, direction.raw, 0.0f, worldDirection.raw);
  return worldDirection;
}

vec3s blxTransform_InverseTransformDirection(const blxTransform* trans, const vec3s direction)
{
  mat4 rot;
  glm_quat_mat4(trans->rotation, rot);
  glm_mat4_inv(rot, rot);

  vec3s localDirection;
  // Multiply the direction by the inverse of the rotation matrix to get the local direction
  glm_mat4_mulv3(rot, direction.raw, 0.0f, localDirection.raw);
  return localDirection;
}

void blxTransform_Forward(blxTransform* trans, vec3 outVec) {
  mat4 rot;
  glm_quat_mat4(trans->rotation, rot);
  glm_mat4_mulv3(rot, Vec3_Forward, 1.0f, outVec);
}

void blxTransform_Up(blxTransform* trans, vec3 outVec) {
  mat4 rot;
  glm_quat_mat4(trans->rotation, rot);
  glm_mat4_mulv3(rot, Vec3_Up, 1.0f, outVec);
}

void blxTransform_Right(blxTransform* trans, vec3 outVec) {
  mat4 rot;
  glm_quat_mat4(trans->rotation, rot);
  glm_mat4_mulv3(rot, Vec3_Right, 1.0f, outVec);
}

void blxTransform_EulerAngles(blxTransform* trans, vec3 outEulers) {
  mat4 model;
  _transform_modelMatrix(trans, model);
  mat4 rot;
  glm_quat_mat4(trans->rotation, rot);
  glm_euler_angles(rot, outEulers);
  outEulers[0] = glm_deg(outEulers[0]);
  outEulers[1] = glm_deg(outEulers[1]);
  outEulers[2] = glm_deg(outEulers[2]);
}

void blxTransform_SetRotationEuler(blxTransform* trans, vec3 eulers) {
  // glm_euler_zxy_quat((vec3) {glm_rad(eulers[0], glm_rad(eulers[1],
  // glm_rad(eulers[2]))) }, trans->rotation); glm_quatv
  // TODO: Fix this!!
}

