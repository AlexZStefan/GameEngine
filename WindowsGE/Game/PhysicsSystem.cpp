#include "PhysicsSystem.h"

namespace GE {
	extern Coordinator gCoordinator;
	static physx::PxReal stackZ = 20.0f; // delete if not calling test function

	//void PhysicsSystem::CreateConvexMesh(const physx::PxVec3* verts, const physx::PxU32 numVerts)
	/* 
	PxRigidStatic* PhysicsSystem::CreateTerrainMesh(const std::vector<physx::PxVec3>* verts, const physx::PxU32 numVerts)
	{
		// Create descriptor for convex mesh
		PxTriangleMeshDesc convexDesc;
		convexDesc.points.count = numVerts;
		convexDesc.points.stride = sizeof(PxVec3);
		convexDesc.points.data = verts->data();
		convexDesc.triangles.stride = 3 * sizeof(PxU32);

		PxTolerancesScale scale;
		PxCookingParams params(scale);

		PxDefaultMemoryOutputStream outStream;
		params.midphaseDesc = PxMeshMidPhase::eBVH33;

		// setup common cooking params
		params.suppressTriangleMeshRemapTable = true;
		params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
		params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
		params.midphaseDesc.mBVH33Desc.meshCookingHint = PxMeshCookingHint::eSIM_PERFORMANCE;

		if (!PxCookTriangleMesh(params, convexDesc, outStream))
			std::cout << "Failed to cook convex mesh";
		PxDefaultMemoryInputData inStream(outStream.getData(), outStream.getSize());
		
		PxRigidStatic* terrain = m_PhysX->createRigidStatic(physx::PxTransform(1, 1, 1));
		//plane = PxCreatePlane(*m_PhysX,  physx::PxPlane(.0f, 0.0f,1.0f, 0), *m_PxDefaultMaterial);
		//plane = *plane, PxPlaneGeometry(), * m_PxDefaultMaterial

		physx::PxTriangleMesh* convexMesh = m_PhysX->createTriangleMesh(inStream);

		PxShape* aConvexShape = PxRigidActorExt::createExclusiveShape(*terrain,
			PxTriangleMeshGeometry(convexMesh), *m_PxDefaultMaterial);
		//PxShape* terrainShape = m_PhysX->createShape(terrain, groundShape, m_PxDefaultMaterial, 1, false);
		//terrain->setGlobalPose(physx::PxTransform(0, -10, 0));
		terrain->attachShape(*aConvexShape);
		m_PxScene->addActor(*terrain);

		return terrain;
	}*/

	PxRigidDynamic* PhysicsSystem::CreateConvexMesh(Model* _model)
	{
		const physx::PxU32 numVerts = _model->vertices.size();
		std::vector<PxVec3>* pxVerts = new std::vector<PxVec3>();
		Model* m = _model;
		for (int i = 0; i < m->vertices.size(); i++) {
			PxVec3 pos;
			Vertex& v = m->vertices[i];
			pos.x = v.x;
			pos.y = v.y;
			pos.z = v.z;
			pxVerts->push_back(pos);
		}

		// Create descriptor for convex mesh
		PxConvexMeshDesc convexDesc;
		convexDesc.points.count = numVerts;
		convexDesc.points.stride = sizeof(PxVec3);
		convexDesc.points.data = pxVerts->data();
		convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;
		//convexDesc.triangles.stride = 3 * sizeof(PxU32);

		PxTolerancesScale scale;
		PxCookingParams params(scale);
		
		// setup common cooking params
		params.suppressTriangleMeshRemapTable = true;
		params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
		params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
		params.midphaseDesc.mBVH33Desc.meshCookingHint = PxMeshCookingHint::eSIM_PERFORMANCE;

		PxConvexMesh* convex = NULL;
		convex = PxCreateConvexMesh(params, convexDesc, m_PhysX->getPhysicsInsertionCallback());
		PxDefaultMemoryOutputStream outStream;
		//params.midphaseDesc = PxMeshMidPhase::eBVH33;
		bool res = PxCookConvexMesh(params, convexDesc, outStream);

		PxDefaultMemoryInputData inStream(outStream.getData(), outStream.getSize());

		PxRigidDynamic* actor = m_PhysX->createRigidDynamic(physx::PxTransform(1, 1, 1));
		//plane = PxCreatePlane(*m_PhysX,  physx::PxPlane(.0f, 0.0f,1.0f, 0), *m_PxDefaultMaterial);
		//plane = *plane, PxPlaneGeometry(), * m_PxDefaultMaterial

		convex = m_PhysX->createConvexMesh(inStream);
		
		PxShape* aConvexShape = PxRigidActorExt::createExclusiveShape(*actor,
			PxConvexMeshGeometry(convex), *m_PxDefaultMaterial);
		//PxShape* terrainShape = m_PhysX->createShape(terrain, groundShape, m_PxDefaultMaterial, 1, false);
		actor->setGlobalPose(physx::PxTransform(_model->transform.position.x, _model->transform.position.y, _model->transform.position.z));
		actor->attachShape(*aConvexShape);
		actor->setName(_model->name.c_str());

		m_PxScene->addActor(*actor);

		delete pxVerts;
		pxVerts = nullptr;
		return actor;
	}	

	// TODO FIX THE INDECES SO THAT THEY CAN BE PASSED CORRECTLY TO MESH DESC
	PxRigidDynamic* PhysicsSystem::CreateTriangleDynamicMesh(Model* _model)
	{
		const physx::PxU32 numVerts = _model->vertices.size();
		std::vector<PxVec3>* pxVerts = new std::vector<PxVec3>();
		Model* m = _model;
		for (int i = 0; i < m->vertices.size(); i++) {
			PxVec3 pos;
			Vertex& v = m->vertices[i];
			pos.x = v.x;
			pos.y = v.y;
			pos.z = v.z;
			pxVerts->push_back(pos);
		}

		// Create descriptor for convex mesh
		PxTriangleMeshDesc triangleMeshDesc;
		triangleMeshDesc.points.count = numVerts;
		triangleMeshDesc.points.stride = sizeof(PxVec3);
		triangleMeshDesc.points.data = pxVerts->data();
		triangleMeshDesc.triangles.count = _model->indexCount;
		triangleMeshDesc.triangles.stride = 3 * sizeof(PxU32);
		//convexDesc.flags = PxConvexFlag::eCOMPUTE_CONVEX;

		PxTolerancesScale scale;
		PxCookingParams params(scale);
		PxDefaultMemoryOutputStream outStream;

		/*	// setup common cooking params
		//params.suppressTriangleMeshRemapTable = true;
		params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
		params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
		params.midphaseDesc.mBVH33Desc.meshCookingHint = PxMeshCookingHint::eSIM_PERFORMANCE;*/
		//params.gaussMapLimit = 256; //16

		if (!PxCookTriangleMesh(params, triangleMeshDesc, outStream));
			std::cout << "Failed to cook convex mesh";
		PxDefaultMemoryInputData inStream(outStream.getData(), outStream.getSize());		
		PxRigidDynamic* actor = m_PhysX->createRigidDynamic(physx::PxTransform(1, 1, 1));
		{
			actor->setRigidBodyFlag(PxRigidBodyFlag::eKINEMATIC, true);
		}

		physx::PxTriangleMesh* triangleMesh = m_PhysX->createTriangleMesh(inStream);

		PxShape* triangleShape = PxRigidActorExt::createExclusiveShape(*actor,
			PxTriangleMeshGeometry(triangleMesh), *m_PxDefaultMaterial);
		actor->setMass(1.0f);
		actor->setMassSpaceInertiaTensor(PxVec3(1.0f, 1.0f, 1.0f));

		actor->setGlobalPose(physx::PxTransform(_model->transform.position.x, _model->transform.position.y, _model->transform.position.z));
		actor->attachShape(*triangleShape);
		actor->setName(_model->name.c_str());

		m_PxScene->addActor(*actor);

		delete pxVerts;
		pxVerts = nullptr;
		return actor;
	}

	PxRigidStatic* PhysicsSystem::CreateTriangleStaticMesh(Model* _model)
	{
		const physx::PxU32 numVerts = _model->vertices.size();
		std::vector<PxVec3>* pxVerts = new std::vector<PxVec3>();
		Model* m = _model;
		for (int i = 0; i < m->vertices.size(); i++) {
			PxVec3 pos;
			Vertex& v = m->vertices[i];
			pos.x = v.x;
			pos.y = v.y;
			pos.z = v.z;
			pxVerts->push_back(pos);
		}

		// Create descriptor for convex mesh
		PxTriangleMeshDesc triangleMeshDesc;
		triangleMeshDesc.points.count = numVerts;
		triangleMeshDesc.points.stride = sizeof(PxVec3);
		triangleMeshDesc.points.data = pxVerts->data();
		triangleMeshDesc.triangles.stride = 3 * sizeof(PxU32);

		PxTolerancesScale scale;
		PxCookingParams params(scale);
		PxDefaultMemoryOutputStream outStream;
		params.midphaseDesc = PxMeshMidPhase::eBVH33;

		// setup common cooking params
		params.suppressTriangleMeshRemapTable = true;
		params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
		params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;
		params.midphaseDesc.mBVH33Desc.meshCookingHint = PxMeshCookingHint::eSIM_PERFORMANCE;

		if (!PxCookTriangleMesh(params, triangleMeshDesc, outStream))
			std::cout << "Failed to cook convex mesh";
		PxDefaultMemoryInputData inStream(outStream.getData(), outStream.getSize());

		PxRigidStatic* actor = m_PhysX->createRigidStatic(physx::PxTransform(1, 1, 1));

		physx::PxTriangleMesh* triangleMesh = m_PhysX->createTriangleMesh(inStream);

		PxShape* triangleShape = PxRigidActorExt::createExclusiveShape(*actor,
			PxTriangleMeshGeometry(triangleMesh), *m_PxDefaultMaterial);

		//PxShape* terrainShape = m_PhysX->createShape(terrain, groundShape, m_PxDefaultMaterial, 1, false);
		actor->setGlobalPose(physx::PxTransform(_model->transform.position.x, _model->transform.position.y, _model->transform.position.z));
		actor->attachShape(*triangleShape);
		actor->setName(_model->name.c_str());
		m_PxScene->addActor(*actor);

		delete pxVerts;
		pxVerts = nullptr;
		return actor;
	}

	/*//physx::PxRigidStatic* CreateTerrain(physx::PxPhysics* physics, physx::PxCooking* cooking, physx::PxMaterial* material) {
	//	// Create terrain actor
	//	physx::PxTransform transform = physx::PxTransform::createIdentity();
	//	physx::PxHeightFieldDesc heightFieldDesc;
	//	heightFieldDesc.nbRows = mapHeight;
	//	heightFieldDesc.nbColumns = mapWidth;
	//	heightFieldDesc.samples.data = reinterpret_cast<const physx::PxHeightFieldSample*>(pixelData);
	//	heightFieldDesc.samples.stride = sizeof(physx::PxHeightFieldSample);
	//	heightFieldDesc.thickness = -10.0f; // adjust this value based on your terrain

	//	physx::PxHeightField* heightField = cooking->createHeightField(heightFieldDesc);
	//	physx::PxHeightFieldGeometry heightFieldGeometry(heightField, physx::PxMeshGeometryFlags(), heightScaling, scaling, scaling);

	//	physx::PxRigidStatic* terrainActor = physics->createRigidStatic(transform);
	//	physx::PxShape* shape = physx::PxRigidActorExt::createExclusiveShape(*terrainActor, heightFieldGeometry, *material);

	//	return terrainActor;
	//}

	//// Creates the object out of the assimp mesh
	//physx::PxConvexMesh* class::MakeObject(int index, Entity* entity)
	//{
	//	physx::PxU32 vertexCount;
	//	std::vector<physx::PxVec3> vertices;
	//	if (entity != nullptr)
	//	{

	//		if (mesh)
	//		{

	//			vertexCount = mesh->GetNumberOfVertices(index);

	//			std::vector<CVector3> meshVertices = mesh->GetVertices();
	//			// Copy from cvector array to PxVec3 array
	//			for (int i = 0; i < vertexCount; i++)
	//			{
	//				vertices.push_back(physx::PxVec3(trackVertices[i].x, trackVertices[i].y, trackVertices[i].z));
	//			}

	//			physx::PxVec3* v = vertices.data();

	//			return CreateConvexMesh(v, vertexCount, m_PhysicsSystem->GetPhysics(), m_PhysicsSystem->GetCooking());
	//		}
	//	}
	//	return nullptr;
	//}*/

	PxRaycastBuffer PhysicsSystem::RayCastToObject(PxVec3 origin, PxVec3 unitDir, float maxDist) {
		const PxU32 maxHits = 1;
		PxHitFlags hitFlags = PxHitFlag::ePOSITION | PxHitFlag::eMESH_ANY | PxHitFlag::eUV; // PxHitFlag::eNORMAL 
		PxQueryFilterData  queryFlags;
		queryFlags.flags |= PxQueryFlag::eANY_HIT;
		//const PxU32 stride = sizeof(PxGeomRaycastHit);
		//const PxGeometryQueryFlags queryFlags = PxGeometryQueryFlag::eDEFAULT;
		//PxRaycastThreadContext* threadContext = NULL;
		PxRaycastHit hit;
		PxRaycastBuffer buffer;
		bool wasHit = m_PxScene->raycast(origin, unitDir, 200, buffer, hitFlags);
		if (wasHit )
		{
			return buffer;
			/*auto &a = buffer.block.position; 
			auto &b = buffer.block.shape; 
			auto& c = buffer.block.actor;
			std::cout << c->getName();
			std::cout << " raycast \n ";
			std::cout << std::to_string( a.x )<< "\n";
			std::cout << std::to_string( a.y )<< "\n";
			std::cout << std::to_string( a.z )<< "\n";
			std::cout << std::to_string( hit.distance )<< "\n";*/
		}
	}

	static void createStack(const physx::PxTransform& t, physx::PxU32 size, physx::PxReal halfExtent, physx::PxPhysics* gPhysics, physx::PxMaterial* gMaterial, physx::PxScene * gScene)
	{
		physx::PxShape* shape = gPhysics->createShape(physx::PxBoxGeometry(halfExtent, halfExtent, halfExtent), *gMaterial);
		for (physx::PxU32 i = 0; i < size; i++)
		{
			for (physx::PxU32 j = 0; j < size - i; j++)
			{
				physx::PxTransform localTm(physx::PxVec3(physx::PxReal(j * 2) - physx::PxReal(size - i), physx::PxReal(i * 2 + 1), 0) * halfExtent);
				physx::PxRigidDynamic* body = gPhysics->createRigidDynamic(t.transform(localTm));
				body->attachShape(*shape);
				physx::PxRigidBodyExt::updateMassAndInertia(*body, 10.0f);
				gScene->addActor(*body);
			}
		}
		shape->release();
	}
		physx::PxPhysics* customCreatePhysics(physx::PxU32 version,
			physx::PxFoundation& foundation,
			const physx::PxTolerancesScale& scale,
			bool trackOutstandingAllocations,
			physx::PxPvd* pvd)
		{
			physx::PxPhysics* physics = PxCreatePhysics(version, foundation, scale,
				trackOutstandingAllocations, pvd);
			if (!physics)
				return NULL;
			//physx::PxRegisterArticulations(*physics);
			//physx::PxRegisterHeightFields(*physics);
			return physics;
		}

	void PhysicsSystem::StartUp()
	{
		m_PxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_PxAllocator,
			m_PXErrorCallback);
		if (!m_PxFoundation)
			std::cout << ("PxCreateFoundation failed!");
		// for visual debugger
		bool recordMemoryAllocations = true;
		m_Pvd = physx::PxCreatePvd(*m_PxFoundation);
		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate(m_pvdIPAddress.c_str(), 5425, 10);
		m_Pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
		// enanble pysics
		m_PhysX = PxCreatePhysics(PX_PHYSICS_VERSION, *m_PxFoundation, physx::PxTolerancesScale(), true, m_Pvd);
		PxInitExtensions(*m_PhysX, m_Pvd);

		physx::PxSceneDesc sceneDesc(m_PhysX->getTolerancesScale());
		sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f); // -9.81f
		/*m_PhysX = customCreatePhysics(PX_PHYSICS_VERSION, *m_PxFoundation,
			physx::PxTolerancesScale(), recordMemoryAllocations, m_Pvd);*/
		if (!PxInitExtensions(*m_PhysX, m_Pvd))
			std::cout << ("PxInitExtensions failed!");

		////This creates CPU dispatcher threads or worker threads. We will make 2
		m_PxDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		sceneDesc.cpuDispatcher = m_PxDispatcher;
		sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

		// GPU
		physx::PxCudaContextManagerDesc cudaContextManagerDesc;
		auto gCudaContextManager = PxCreateCudaContextManager(*m_PxFoundation, cudaContextManagerDesc, PxGetProfilerCallback());
		sceneDesc.cudaContextManager = gCudaContextManager;
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_GPU_DYNAMICS;	//Enable GPU dynamics - without this enabled, simulation (contact gen and solver) will run on the CPU.
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_PCM;			//Enable PCM. PCM NP is supported on GPU. Legacy contact gen will fall back to CPU
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_STABILIZATION;	//Improve solver stability by enabling post-stabilization.
		sceneDesc.broadPhaseType = physx::PxBroadPhaseType::eGPU; //Enable GPU broad phase. Without this set, broad phase will run on the CPU.
		sceneDesc.gpuMaxNumPartitions = 8;	//Defines the maximum number of partitions used by the solver. Only power-of-2 values are valid. 
		sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS;

		////Create the scene now by passing the scene's description
		m_PxScene = m_PhysX->createScene(sceneDesc);
		
		physx::PxPvdSceneClient* pvdClient = m_PxScene->getScenePvdClient();
		if (pvdClient)
		{
			// set some flags that it needs for visual debbuging
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
			pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		}
		//m_PxDefaultMaterial = m_PhysX->createMaterial(m_defaultStaticFriction, m_defaultDynamicFriction, m_defaultRestitution);
		m_PxDefaultMaterial = m_PhysX->createMaterial(0.5, 0.5, 0.6);
		////////////////////////////////// TO ADD LATER //////////////////////////////////	 
		//PxSetCookingParams *m_PxCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_PxFoundation, 0.06f * physx::PxTolerancesScale.length * physx::PxTolerancesScale.length);
		////if (!m_PxCooking)
		//std::Cout << ("PxCreateCooking failed!");
#pragma region old;
		////PhysX starts off by setting up a Physics Foundation
		//m_PxFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_PxAllocator, m_PXErrorCallback);

		////Setup PhysX cooking if you need convex hull cooking support or other cooking features
		//m_PxCooking = PxCreateCooking(PX_PHYSICS_VERSION, *m_PxFoundation, physx::PxCookingParams(physx::PxTolerancesScale()));

		////Create the PhysX Visual Debugger by giving it the current foundation
		//m_Pvd = PxCreatePvd(*m_PxFoundation);
		////The PVD needs connection via a socket. It will run on the Address defined, in our case it's our machine
		//physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate(m_pvdIPAddress.c_str(), m_pvdPortNumber, m_pvdTimeOutSeconds);
		//m_Pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

		////Create Physics! This creates an instance of the PhysX SDK
		//m_PhysX = PxCreatePhysics(PX_PHYSICS_VERSION, *m_PxFoundation, physx::PxTolerancesScale(), true, m_Pvd);
		//PxInitExtensions(*m_PhysX, m_Pvd);

		////What is the description of this PhysX scene?
		//physx::PxSceneDesc sceneDesc(m_PhysX->getTolerancesScale());
		//sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
		////This creates CPU dispatcher threads or worker threads. We will make 2
		//m_PxDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
		//sceneDesc.cpuDispatcher = m_PxDispatcher;
		//sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;
		////Create the scene now by passing the scene's description
		//m_PxScene = m_PhysX->createScene(sceneDesc);

		//physx::PxPvdSceneClient* pvdClient = m_PxScene->getScenePvdClient();
		//if (pvdClient)
		//{
		//	//I have a PVD client, so set some flags that it needs
		//	pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		//	pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		//	pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		//}
		//m_PxDefaultMaterial = m_PhysX->createMaterial(m_defaultStaticFriction, m_defaultDynamicFriction, m_defaultRestitution);
#pragma endregion
	}

	PxRigidStatic* PhysicsSystem::CreateStaticActor() {
		return m_PhysX->createRigidStatic(physx::PxTransform(1, 1, 1));
	}

	int PhysicsSystem::GetEntityFromBuffer(PxRigidActor* _actor) {
		int selectEntity = -1;

		for (auto const& entity : mEntities)
		{
			auto& actor = gCoordinator.GetComponent<Physics>(entity).actor;
			if (actor == _actor) {
				selectEntity = entity;
			}
		}
		std::cout << "selected entity # " << selectEntity << "\n";

		return selectEntity;
	}

	void PhysicsSystem::Update(float deltaTime)
	{
		m_PxScene->simulate(deltaTime);
		m_PxScene->fetchResults(true);
		for (auto const& entity : mEntities)
		{
			auto& physics = gCoordinator.GetComponent<Physics>(entity);
			auto& transform = gCoordinator.GetComponent<Transform>(entity);
			auto& mod = gCoordinator.GetComponent<Model>(entity);

			PxTransform pxTransform = transform;
				physics.actor->setGlobalPose(pxTransform);

				/*if (physics.isKinematic) {
					PxTransform actorTransform = physics.actor->getGlobalPose();
					actorTransform.p.x = transform.position.x;
					transform.position.x += 10 * deltaTime;
					physics.actor->setGlobalPose(actorTransform);
				}*/
		}
	}

	void PhysicsSystem::ShutDown()
	{
		PX_RELEASE(m_PxScene);
		PX_RELEASE(m_PxDispatcher);
		PX_RELEASE(m_PhysX);
		//PX_RELEASE(m_PxCooking);

		if (m_Pvd)
		{
			physx::PxPvdTransport* transport = m_Pvd->getTransport();
			m_Pvd->release();	m_Pvd = NULL;
			PX_RELEASE(transport);
		}

		//Release the PhysX foundation last as other things depend on it
		PX_RELEASE(m_PxFoundation);
	}
}
