
#include "Camera.h"

GE::Camera::Camera(glm::vec3 cam_pos, glm::vec3 cam_target, glm::vec3 cam_upDir, float fov, float ar, float near, float far)
{
	pos = cam_pos;
	target = cam_target;
	camUp_dir = cam_upDir;
	fovy = fov;
	aspectRatio = ar;
	nearClip = near;
	farClip = far;


}

void GE::Camera::updateCamMatrices()
{
	viewMat = glm::lookAt(pos, target, camUp_dir);
	projectionMat = glm::perspective(glm::radians(fovy), aspectRatio, nearClip, farClip);


}

