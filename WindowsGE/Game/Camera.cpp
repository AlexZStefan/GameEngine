#include "Camera.h"

namespace GE {
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
		oldMouseX =0, oldMouseY = 0;
	}

	GE::Camera::Camera()
	{
		float windowHeight = 960.0f;
		float windowWidth = 1280.0f;

		pos = glm::vec3(.0f, .0f, 0.0f);
		target = glm::vec3(.0f, 5.0f, 1.0f);
		up_vector = glm::vec3(.0f, 1.0f, .0f);
		fovy = 45.0f;
		aspectRatio = windowWidth / windowHeight;
		nearClip = 0.1f;
		farClip = 10000.0f;
		oldMouseX =0, oldMouseY = 0;
	}

	Vec3f Camera::ComputeRayDirection(glm::vec3 worldCoords) {
		// Camera position in world coordinates
		glm::mat4 invViewMat = glm::inverse(viewMat);
		glm::vec3 cameraPosition(invViewMat[3]);

		// Ray direction vector towards the specified world coordinates
		glm::vec3 rayDirection = glm::normalize(worldCoords - cameraPosition);

		return Vec3f(rayDirection.x, rayDirection.y, rayDirection.z);
	}

	Vec3f Camera::ScreenToWorld(int mouseX, int mouseY, int windowWidth, int windowHeight) {
		// Normalize mouse coordinates to the range [-1, 1] NDC
		float normalizedX = (2.0f * mouseX) / windowWidth - 1.0f;
		float normalizedY = 1.0f - (2.0f * mouseY) / windowHeight;

	
		glm::vec4 clipCoords(normalizedX, normalizedY, -1.0f, 1.0f);

		// Inverse projection matrix
		glm::mat4 invProjMat = glm::inverse(projectionMat);

		// Eye coordinates
		glm::vec4 eyeCoords = invProjMat * clipCoords;
		eyeCoords.z = -1.0f;
		// Inverse view matrix
		glm::mat4 invViewMat = glm::inverse(viewMat);

		// World coordinates
		glm::vec4 worldCoords = invViewMat * eyeCoords;
		worldCoords /= worldCoords.w;

		return Vec3f (worldCoords.x, worldCoords.y, worldCoords.z);
	}

	void GE::Camera::updateCamMatrices()
	{
		viewMat = glm::lookAt(pos, pos + target, up_vector);
		projectionMat = glm::perspective(glm::radians(fovy), aspectRatio, nearClip, farClip);

		//std::cout << "pitch " << pitch << " yah " << yaw << "\n";
	}

}