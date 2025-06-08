#include "rendering/blx_camera.h"

void blxCamera_Init(Camera* cam, float fov)
{
	blxTransform_Init(&cam->transform);
	glm_mat4_identity(cam->viewMatrix);
	glm_mat4_identity(cam->projecionMatrix);
	cam->fov = fov;
	blxCamera_SetProjection(cam, PERSPECTIVE);

}

void blxCamera_SetProjection(Camera* cam, CameraProjectionType projType)
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

void _blxGetCameraProjection(Camera* cam, CameraProjectionType projType, mat4 dest)
{
	switch (projType)
	{
		case PERSPECTIVE:
			//TODO: Temp aspect ratio for now, will update later.
			glm_perspective(glm_rad(cam->fov), 960.0f / 540.0f, 0.1f, 100.0f, dest);
			break;
		case ORTHOGRAPHIC:
			//TODO: TEMP VALUES FOR NOW 
			glm_ortho(0, 960, 540, 0, -1, 1, dest);
			break;
	}
}


