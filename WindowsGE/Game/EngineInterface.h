#pragma once
#include "systems/ECS/ECS.h"
#include "imgui/imgui.h"
#include "PhysicsSystem.h"
#include "Functions.h"
#include "Camera.h"
#include "Camera.h"

#include <glm/common.hpp>
#include <unordered_map>
#include <optional>
namespace GE {
	class EngineInterface
	{
	public:
		void EngineInterfaceInit(Shared<Camera> _camera,  Shared<PhysicsSystem> _physicsSystem) {
			physicsSystem = _physicsSystem;
			camera = _camera;
		}
		int currentSelected = -1;

		void SelectObject(Entity entity) {
			currentSelected = entity;
		}

		/// <summary>
		/// cast for objects in scene
		/// </summary>
		/// <param name="mouse_x"></param>
		/// <param name="mouse_y"></param>
		/// <param name="w"></param>
		/// <param name="h"></param>
		void EntityRaycast(float mouse_x, float mouse_y, float w, float h);

		void DisplayCurrentSellectedData(float windowWidth, float windowHeight, std::vector<Entity>& gameEntities);

	private:
		std::vector<IComponentArray> entityComponents;
		Shared <PhysicsSystem> physicsSystem;
		Shared<Camera> camera;
	};
}


