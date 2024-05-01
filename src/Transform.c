#include "Transform.h"
#include "Defines.h"

void transform_init(Transform* trans) {
  glm_vec3(GLM_VEC3_ZERO, trans->position);
  glm_vec3(GLM_VEC3_ONE, trans->scale);
  glm_quat_identity(trans->rotation);
}

void _transform_modelMatrix(Transform* trans, mat4 outModel) {
  mat4 t, r, s;
  glm_translate_make(t, trans->position);
  glm_quat_mat4(trans->rotation, r);
  glm_scale_make(s, trans->scale);
  mat4 tr;
  glm_mat4_mul(t, r, tr);
  glm_mat4_mul(s, tr, outModel);
}

void transform_forward(Transform* trans, vec3 outVec) {
  mat4 rot;
  glm_quat_mat4(trans->rotation, rot);
  glm_mat4_mulv3(rot, Vec3_Forward, 1.0f, outVec);
}

void transform_up(Transform* trans, vec3 outVec) {
  mat4 rot;
  glm_quat_mat4(trans->rotation, rot);
  glm_mat4_mulv3(rot, Vec3_Up, 1.0f, outVec);
}

void transform_right(Transform* trans, vec3 outVec) {
  mat4 rot;
  glm_quat_mat4(trans->rotation, rot);
  glm_mat4_mulv3(rot, Vec3_Right, 1.0f, outVec);
}

void transform_eulerAngles(Transform* trans, vec3 outEulers) {
  mat4 model;
  _transform_modelMatrix(trans, model);
  mat4 rot;
  glm_quat_mat4(trans->rotation, rot);
  glm_euler_angles(rot, outEulers);
  outEulers[0] = glm_deg(outEulers[0]);
  outEulers[1] = glm_deg(outEulers[1]);
  outEulers[2] = glm_deg(outEulers[2]);
}

void transform_setRotationEuler(Transform* trans, vec3 eulers) {
  // glm_euler_zxy_quat((vec3) {glm_rad(eulers[0], glm_rad(eulers[1],
  // glm_rad(eulers[2]))) }, trans->rotation); glm_quatv
  // TODO: Fix this!!
}

// glm_euler_xyz_quat()
