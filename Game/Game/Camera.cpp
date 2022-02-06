
#include "Camera.h"

GE::Camera::Camera(glm::vec3 cam_pos, glm::vec3 cam_target, glm::vec3 cam_upDir, float fov, float ar, float near, float far)
{
	pos = cam_pos;
	target = cam_target;
	up_vector = cam_upDir;
	fovy = fov;
	aspectRatio = ar;
	nearClip = near;
	farClip = far;

	updateCamMatrices();
}

void GE::Camera::updateCamMatrices()
{
	viewMat = glm::lookAt(pos, target, up_vector);
	projectionMat = glm::perspective(glm::radians(fovy), aspectRatio, nearClip, farClip);


}

