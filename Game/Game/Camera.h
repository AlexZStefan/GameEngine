#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <SDL.h>

namespace GE {

	class Camera
	{
	public:
		Camera(glm::vec3 cam_pos, glm::vec3 cam_target, glm::vec3 cam_upDir, float fov, float ar, float near, float far);

		~Camera() {};

		float getPosX() { return pos.x; }
		float getPosY() { return pos.y; }
		float getPosZ() { return pos.z; }

		glm::vec3 getTarget() { return target; }
		glm::vec3 getUpDir() { return up_vector; }
		glm::mat4 getViewMatrix() { return viewMat; }
		glm::mat4 getProjectionMatrix() { return projectionMat; }

		void setPosX(float x) {
			pos = glm::vec3(x, pos.y, pos.z);
			updateCamMatrices();
		}
		void setPosY(float y) {
			pos = glm::vec3(pos.x, y, pos.z);
			updateCamMatrices();
		}

		void setPosZ(float z) {
			pos = glm::vec3(pos.x, pos.y, z);
			updateCamMatrices();
		}

		void setPos(glm::vec3 newPos) {
			pos = newPos;
			updateCamMatrices();
		}

		void setTarget(glm::vec3 newTarget) {
			target = newTarget;
			updateCamMatrices();
		}

		void setUpDir(glm::vec3 newUp) {
			up_vector = newUp;
			updateCamMatrices();
		}

		void setFov(float newFov) {
			fovy = newFov;
			updateCamMatrices();
		}

		void setAspectRatio(float newAratio) {
			aspectRatio = newAratio;
			updateCamMatrices();
		}

		void setNearClip(float newNearClip)
		{
			nearClip = newNearClip;
			updateCamMatrices();
		}

		void setFarClip(float newFarClip)
		{
			farClip = newFarClip;
			updateCamMatrices();
		}

		void setPitch(float newPitch) {
			pitch = newPitch;

			// clamped value so that camera does not rotate
			if (pitch > 90.0f)pitch = 90.0f;
			if (pitch < -90.0f)pitch = -90.0f;
		};

		void setYaw(float newYaw) {
			yaw = newYaw;
		};

		void setOldMouseX(float newOldMouseX) {
			newOldMouseX = newOldMouseX;
		};

		void setOldMouseY(float newOldMouseY) {
			oldMouseY = newOldMouseY;
		};

		float getPitch() {
			return pitch;
		}

		float getYaw() {
			return yaw;
		}

		float getMouseX() {
			return oldMouseX;
		}

		float getMouseY() {
			return oldMouseY;
		}

		glm::vec3 getPos() {
			return pos;
		}

	private:
		glm::vec3 pos;
		glm::vec3 target;
		glm::vec3 up_vector;

		//glm::vec4 viewMat;
		float fovy;
		float aspectRatio;
		float nearClip;
		float farClip;

		float pitch = 0.0f;
		float yaw = -90.0f;

		int oldMouseX, oldMouseY;

		glm::mat4 viewMat;
		glm::mat4 projectionMat;

		void updateCamMatrices();
	};
}