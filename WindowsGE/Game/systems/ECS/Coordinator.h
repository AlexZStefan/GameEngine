#pragma once
#include "ECS.h"
#include "ComponentManager.h"
#include "SystemManager.h"
#include "EntityManager.h"

namespace GE{
	class Coordinator
	{
	public:
		void Init()
		{
			// Create pointers to each manager
			mComponentManager = std::make_unique<ComponentManager>();
			mEntityManager = std::make_unique<EntityManager>();
			mSystemManager = std::make_unique<SystemManager>();
		}

		// Entity methods
		Entity CreateEntity()
		{
			int entity = mEntityManager->CreateEntity();
			gameEntities.push_back(entity);
			return entity;
		}

		void DestroyEntity(Entity entity)
		{
			mEntityManager->DestroyEntity(entity);

			mComponentManager->EntityDestroyed(entity);

			mSystemManager->EntityDestroyed(entity);

			auto it = std::find(gameEntities.begin(), gameEntities.end(), entity);
			if (it != gameEntities.end()) {
				// Erase the element from the vector
				gameEntities.erase(it);

				std::cout << "Element removed successfully." << std::endl;
			}
			else {
				std::cout << "Element not found in the vector." << std::endl;
			}
		}

		// Component methods
		template<typename T>
		void RegisterComponent()
		{
			mComponentManager->RegisterComponent<T>();
		}

		template<typename T>
		T& AddComponent(Entity entity, T component)
		{
			mComponentManager->AddComponent<T>(entity, component);
			auto signature = mEntityManager->GetSignature(entity);
			signature.set(mComponentManager->GetComponentType<T>(), true);
			mEntityManager->SetSignature(entity, signature);
			mSystemManager->EntitySignatureChanged(entity, signature);
			return mComponentManager->GetComponent<T>(entity);
		}

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			mComponentManager->RemoveComponent<T>(entity);

			auto signature = mEntityManager->GetSignature(entity);
			signature.set(mComponentManager->GetComponentType<T>(), false);
			mEntityManager->SetSignature(entity, signature);
			mSystemManager->EntitySignatureChanged(entity, signature);
		}

		template<typename T>
		T& GetComponent(Entity entity)
		{
			return mComponentManager->GetComponent<T>(entity);
		}
		
		template<typename T>
		bool HasComponent(Entity entity)
		{
			return mComponentManager->HasComponent<T>(entity);
		}
	
		template<typename T>
		void GetComponents(Entity entity) {
			mComponentManager->GetAllComponent<T>(entity);
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			return mComponentManager->GetComponentType<T>();
		}

		// System methods
		template<typename T>
		std::shared_ptr<T> RegisterSystem()
		{
			return mSystemManager->RegisterSystem<T>();
		}

		template<typename T>
		void SetSystemSignature(Signature signature)
		{
			mSystemManager->SetSignature<T>(signature);
		}

		std::vector<Entity> gameEntities;

	private:
		std::unique_ptr<ComponentManager> mComponentManager;
		std::unique_ptr<EntityManager> mEntityManager;
		std::unique_ptr<SystemManager> mSystemManager;
	};

}