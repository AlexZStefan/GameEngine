#pragma once
#include <iostream>
#include "phxinclude/PxPhysicsAPI.h"
#include "systems/ECS/Coordinator.h"
#include "Structs.h"
#include "Model.h"

#define PX_RELEASE(x)	if(x)	{ x->release(); x = NULL;	}

namespace GE {

//#if ( defined( _WIN64 ) & defined( _DEBUG ) )
//#pragma comment( lib, "C:/Projects/WindowsGE/Game/Externals/phxlib/debug_x64/PhysX_64.lib" )
//#pragma comment( lib, "C:/Projects/WindowsGE/Game/Externals/phxlib/debug_x64/PhysXCommon_64.lib" )
//#pragma comment( lib, "C:/Projects/WindowsGE/Game/Externals/phxlib/debug_x64/PhysXCooking_64.lib" )
//#pragma comment( lib, "C:/Projects/WindowsGE/Game/Externals/phxlib/debug_x64/PhysXExtensions_static_64.lib" )
//#pragma comment( lib, "C:/Projects/WindowsGE/Game/Externals/phxlib/debug_x64/PhysXFoundation_64.lib" )
//#pragma comment( lib, "C:/Projects/WindowsGE/Game/Externals/phxlib/debug_x64/PhysXPvdSDK_static_64.lib" )
//#pragma comment( lib, "C:/Projects/WindowsGE/Game/Externals/phxlib/debug_x64/PhysXVehicle_static_64.lib" )
//#elif ( defined ( _WIN64 ) & defined( NDEBUG ) )
//#pragma comment( lib, "Externals/phxlib/release_x64/PhysX_64.lib" )
//#pragma comment( lib, "Externals/phxlib/release_x64/PhysXCommon_64.lib" )
//#pragma comment( lib, "Externals/phxlib/release_x64/PhysXCooking_64.lib" )
//#pragma comment( lib, "Externals/phxlib/release_x64/PhysXExtensions_static_64.lib" )
//#pragma comment( lib, "Externals/phxlib/release_x64/PhysXFoundation_64.lib" )
//#pragma comment( lib, "Externals/phxlib/release_x64/PhysXPvdSDK_static_64.lib" )
//#pragma comment( lib, "Externals/phxlib/release_x64/PhysXVehicle_static_64.lib" )
//#endif
	static physx::PxErrorCallback* gDefaultErrorCallback;
	static physx::PxAllocatorCallback* gPxAllocatorCallback;
	static physx::PxScene* m_PxScene;

	class PhysicsSystem : public System {
	public:
		PhysicsSystem() {}
		// Variables we will need
		physx::PxDefaultAllocator					m_PxAllocator;
		physx::PxDefaultErrorCallback				m_PXErrorCallback;

		physx::PxFoundation*						m_PxFoundation ;
		//physx::PxCooking*							m_PxCooking = nullptr;
		physx::PxPhysics*							m_PhysX ;

		physx::PxDefaultCpuDispatcher*				m_PxDispatcher;

		physx::PxMaterial*							m_PxDefaultMaterial ;

		//PhysX Visual Debugger
		physx::PxPvd*								m_Pvd = nullptr;
		std::string							m_pvdIPAddress = "127.0.0.1";
		int									m_pvdPortNumber = 5425;
		std::uint32_t						m_pvdTimeOutSeconds = 10;

		//Default values for PhysX properties to use
		float								m_defaultStaticFriction = 0.5f;
		float								m_defaultDynamicFriction = 0.5f;
		float								m_defaultRestitution = 0.6f;
		float								m_defaultDensity = 10.f;
		float								m_defaultAngularDamping = 0.5f;
		PxTransform	actorTransform;
		physx::PxRigidDynamic* aCapsuleActor;
		physx::PxRigidStatic* plane;
		physx::PxShape* aCapsuleShape;

		//PxRigidStatic* CreateTerrainMesh(const std::vector<physx::PxVec3>* verts, const physx::PxU32 numVerts);
		PxRigidDynamic* CreateConvexMesh(Model* _model);
		PxRigidDynamic* CreateTriangleDynamicMesh(Model* _model);
		PxRigidStatic* CreateTriangleStaticMesh(Model* _model);
		PxRaycastBuffer RayCastToObject(PxVec3 origin, PxVec3 unitDir, float maxDist);
		/*	PxRigidDynamic* CreateConvexDynamicMesh(Model* _model);
		PxRigidDynamic* CreateDynamicRB(Model* _model);
		PxRigidDynamic* CreateTriangleMesh(Model* _model);
		void CreateConvexStaticMesh(Model* _model);*/
		void StartUp();
		PxRigidStatic* CreateStaticActor();
		int GetEntityFromBuffer(PxRigidActor* _actor);
		void Test();
		void Update(float deltaTime) ;

		void ShutDown() ;
	};
};