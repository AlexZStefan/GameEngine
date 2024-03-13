#pragma once
#include "Functions.h"
#include "Model.h"
#include "Billboard.h"
#include "systems/ECS/Coordinator.h"
#include "PhysicsSystem.h"

namespace GE {
	class ObjectManager
	{
	public:
		ObjectManager();
		~ObjectManager();

		Entity CreateObject();
		void DeleteObject(Entity& entity);
		void AddModel(Entity& entity, Shared<Model> _model);
		template<typename T>
		void RemoveComponent(Entity& entity, T component);
		Shared<std::vector<Shared<Object>>> objects;
		template<typename T>
		T& AddComponent(Entity& e, T component);

	private:
		Shared<Shader> standardShader;
	};
}