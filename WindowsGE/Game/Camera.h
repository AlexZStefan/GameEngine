#pragma once

#include "Structs.h"
#include "systems/ECS/SystemManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <SDL.h>

namespace GE {
	class Camera : public System
	{
	public:
		Camera(glm::vec3 cam_pos, glm::vec3 cam_target, glm::vec3 cam_upDir, float fov, float ar, float near, float far);
		Camera();
		~Camera() {};

		float getPosX() { return pos.x; }
		float getPosY() { return pos.y; }
		float getPosZ() { return pos.z; }

		glm::vec3 getTarget() { return target; }
		glm::vec3 getUpDir() { return up_vector; }
		glm::mat4 getViewMatrix() { return viewMat; }
		glm::mat4 getProjectionMatrix() { return projectionMat; }
		Vec3f ComputeRayDirection(glm::vec3 worldCoords);

		Vec3f ScreenToWorld(int mouseX, int mouseY, int windowWidth, int windowHeight);
#pragma region Set/Get Cam Functions

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
			if (pitch > 89.5)pitch = 89.5f;
			if (pitch < -89.5f)pitch = -89.5f;
			updateCamMatrices();
		};

		void setYaw(float newYaw) {
			yaw = newYaw;
			if (yaw < 0)
				yaw += 360.0f;
			if (yaw >= 360.0f)
				yaw -= 360.0f;

			std::cout << "yaw " << yaw << "\n";
			updateCamMatrices();
		};

		void setOldMouseX(float newOldMouseX) {
			oldMouseX = newOldMouseX;
		};

		void setOldMouseY(float newOldMouseY) {
			oldMouseY = newOldMouseY;
		};

		int getOldMouseX() {
			return oldMouseX;
		};

		int getOldMouseY() {
			return oldMouseY ;
		};

		float getPitch() {
			return pitch;
		}

		float getYaw() {
			return yaw;
		}

		glm::vec3 getPos() {
			return pos;
		}

		Vec3f camTarget() {
			return Vec3f(target.x, target.y, target.z);
		}
#pragma endregion

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
		float yaw = 0.f;

		int oldMouseX, oldMouseY;

		glm::mat4 viewMat;
		glm::mat4 projectionMat;

		void updateCamMatrices();
};
}