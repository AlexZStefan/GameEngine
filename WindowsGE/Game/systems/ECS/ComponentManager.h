#pragma once
#include "ECS.h"
#include "IComponentArray.h"

 namespace GE {
	class ComponentManager
	{
	public:
		template<typename T>
		void RegisterComponent()
		{
			const char* typeName = typeid(T).name();

			assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Registering component type more than once.");

			// Add this component type to the component type map
			mComponentTypes.insert({ typeName, mNextComponentType });

			// Create a ComponentArray pointer and add it to the component arrays map
			mComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

			// Increment the value so that the next component registered will be different
			++mNextComponentType;
		}

		template<typename T>
		ComponentType GetComponentType()
		{
			const char* typeName = typeid(T).name();

			assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

			// Return this component's type - used for creating signatures
			return mComponentTypes[typeName];
		}

		template<typename T>
		void AddComponent(Entity entity, T component)
		{
			// Add a component to the array for an entity
			GetComponentArray<T>()->InsertData(entity, component);
		}

		template<typename T>
		void RemoveComponent(Entity entity)
		{
			// Remove a component from the array for an entity
			GetComponentArray<T>()->RemoveData(entity);
		}

		template<typename T>
		T& GetComponent(Entity entity)
		{
			// Get a reference to a component from the array for an entity
			return GetComponentArray<T>()->GetData(entity);
		}

		template<typename T>
		bool HasComponent(Entity entity) {
			return GetComponentArray<T>()->HasData(entity);
		}
		

		template<typename T>
		void GetAllComponent(Entity entity)
		{
			std::vector<ComponentType> components; 

			//for (auto& i : mComponentArrays) {
			//	std::cout << i.first;

			//	const char* typeName = i.first;

			//	auto & ad = mComponentTypes[typeName];  //mComponentArrays[i.first];

			//	std::cout << "ASD";
			//}


			for (auto& i : mComponentTypes) {
				std::cout << i.first;

				const char* typeName = i.first;
				auto& h = typeid(typeName);
				//auto& ad = std::static_pointer_cast<ComponentArray>(mComponentArrays[typeName]);  //mComponentArrays[i.first];
				//static ComponentArray ad = mComponentArrays[typeName];  //mComponentArrays[i.first];
				
				std::cout << "ASD";
			}
			//std::vector<Shared<IComponentArray>> asd; 
			//const char* typeName = typeid(T).name();

			//for (int i = 0; i < mComponentArrays.size(); i++ ) {

			//	auto st = mComponentTypes.find(i);
			////	auto sa = mComponentArrays[mComponentTypes[i]];
			//	//ComponentType hfd = GetComponentArray<T>();
			//	int a = 1;
			//}

			//for (auto & c : mComponentArrays) {
			//	const char* typeNames = typeid(c.first).name();
			//	const std::type_info& intTypeInfo = typeid(c);
			//	asd.push_back(c.second);
			//	mComponentArrays[typeName];
			//}
		//	auto& asd = new std::unordered_map<const char*, std::shared_ptr<IComponentArray>>();
		}

		void EntityDestroyed(Entity entity)
		{
			// Notify each component array that an entity has been destroyed
			// If it has a component for that entity, it will remove it
			for (auto const& pair : mComponentArrays)
			{
				auto const& component = pair.second;

				component->EntityDestroyed(entity);
			}
		}

	private:
		// Map from type string pointer to a component type
		std::unordered_map<const char*, ComponentType> mComponentTypes{};

		// Map from type string pointer to a component array
		std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};

		// The component type to be assigned to the next registered component - starting at 0
		ComponentType mNextComponentType{};

		// Convenience function to get the statically casted pointer to the ComponentArray of type T.
		template<typename T>
		std::shared_ptr<ComponentArray<T>> GetComponentArray()
		{
			const char* typeName = typeid(T).name();

			assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use. " );

			return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
		}
	};
}