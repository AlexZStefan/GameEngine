#include "ObjectManager.h"
namespace GE {
	extern Coordinator gCoordinator;

	ObjectManager::ObjectManager()
	{
	}

	ObjectManager::~ObjectManager()
	{
	}

	Entity ObjectManager::CreateObject()
	{
		auto entity = gCoordinator.CreateEntity();
		gCoordinator.AddComponent(entity, Transform{});
		return entity;
	}

	void ObjectManager::DeleteObject(Entity& entity)
	{
		gCoordinator.DestroyEntity(entity);
	}
	
	void ObjectManager::AddModel(Entity &entity, Shared<Model> _model)
	{
		gCoordinator.AddComponent(entity, Model(standardShader));
		auto& model = gCoordinator.GetComponent<Model>(entity);
		model.SetModel(_model);
	}

	template<typename T>
	T& ObjectManager::AddComponent(Entity& e, T component) {
		gCoordinator.AddComponent(e, component);
		
		return gCoordinator.GetComponent<T>(e);
	}

	template<typename T>
	void ObjectManager::RemoveComponent(Entity& entity, T component)
	{
		gCoordinator.RemoveComponent(entity, component);
	}
}