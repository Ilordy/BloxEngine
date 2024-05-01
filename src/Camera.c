#include "Camera.h"

void camera_init(Camera* cam, float fov)
{
	transform_init(&cam->transform);
	glm_mat4_identity(cam->viewMatrix);
	glm_mat4_identity(cam->projecionMatrix);
	cam->fov = fov;
	camera_setProjection(cam, PERSPECTIVE);
}

void camera_setProjection(Camera* cam, CameraProjectionType projType)
{
	switch (projType)
	{
	case PERSPECTIVE:
		//TODO: Temp aspect ratio for now, will update later.
		glm_perspective(glm_rad(cam->fov), 960.0f / 540.0f, 0.1f, 100.0f, cam->projecionMatrix);
		break;
	case ORTHOGRAPHIC:
		//TODO: uh will do later.
		break;
	}
}

void camera_update(Camera* cam)
{
	mat4 trans;
	_transform_modelMatrix(&cam->transform, trans);
	glm_mat4_copy(trans, cam->viewMatrix);
	glm_mat4_inv(cam->viewMatrix, cam->viewMatrix);
}
