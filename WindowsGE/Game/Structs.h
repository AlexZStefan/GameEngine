#pragma once
#include "Texture.h"
#include "VertexArray.h"
#include "Functions.h"
#include "phxinclude/PxPhysicsAPI.h"

using namespace physx;

namespace GE {
	struct Player {
	public:
		float health;
		float power;
		float maxStats;
	};

	struct Vertex;

	template<typename T>
	struct Vec3 {
		T x, y, z;
		Vec3(T _x, T _y, T _z) {
			x = _x;
			y = _y;
			z = _z;
		}

		Vec3 operator +(const Vec3& other) const
		{
			return Vec3(x + other.x, y + other.y, z + other.z);
		}

		Vec3 operator -(const Vec3& other) const
		{
			return Vec3(x - other.x, y - other.y, z - other.z);
		}

		Vec3 operator *(const Vec3& other) const
		{
			return Vec3(x * other.x, y * other.y, z * other.z);
		}

		Vec3 operator /(const Vec3& other) const
		{
			return Vec3(x / other.x, y / other.y, z / other.z);
		}

		Vertex& operator=(const PxVec3& other) {
			// Assign members appropriately
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			// Assign other members if needed
			return *this;
		}
		
	};

	using Vec3f = Vec3<float>;
	using Vec3i = Vec3<int>;

	struct Gravity
	{
	public:
		Vec3f force;
	};

	/*template<typename T>
	struct RigidBody
	{
		RigidBody() {};
		RigidBody<T>(T bodyType) {
			pxRigidBody = bodyType;
		};
		T pxRigidBody;
		Vec3f velocity = Vec3<float>(0, 0, 0);
		Vec3f acceleration = Vec3<float>(0, 0, 0);
	};*/

	struct RigidBody
	{
		Vec3f velocity = Vec3<float>(0, 0, 0);
		Vec3f acceleration = Vec3<float>(0, 0, 0);
	};

	struct Physics {
#pragma region shape classes 
		/*class PxBoxGeometry;
		class PxSphereGeometry;
		class PxCapsuleGeometry;
		class PxPlaneGeometry;
		class PxConvexMeshGeometry;
		class PxTriangleMeshGeometry;
		class PxTetrahedronMeshGeometry;
		class PxHeightFieldGeometry;
		class PxParticleSystemGeometry;
		class PxHairSystemGeometry;
		class PxRigidActor;
		struct PxFilterData;
		class PxBaseMaterial;
		class PxMaterial;
		class PxFEMSoftBodyMaterial;
		class PxFEMClothMaterial;*/
#pragma endregion
		
		template <typename T>
		void AddShape(T _shape) 
		{
			shape = _shape;
		};
		template <typename T>
		void AddGeometrt(T _geometry) {
			geometry = _geometry;
		}
		template <typename T>
		void AddActor(T _actor) {
			actor = _actor;
		}

		physx::PxRigidActor* actor = nullptr;
		physx::PxShape* shape = nullptr;
		physx::PxShape* geometry = nullptr;

		bool isStatic; 
		bool isKinematic;
	};


	struct Transform {
	public:
		//Transform() {};
		//Transform(Vec3f _pos = Vec3f(0,0,0), Vec3f _scale = Vec3f(0,0,0), Vec3f _rot = Vec3f(0,0,0)) {
		//	position = _pos;
		//	scale = _scale;
		//	rotation = _rot;
		//};

		operator physx::PxTransform() const {
			physx::PxVec3 pos(position.x, position.y, position.z);
			physx::PxQuat quat; 
			quat.createPxQuatFromEulerAngles(rotation.x, rotation.y,rotation.z);
			return physx::PxTransform(pos, quat);
		}

		Transform& operator =(physx::PxTransform &t) {
			position.x = t.p.x;
			position.y = t.p.y;
			position.z = t.p.z;

			rotation.x = t.q.x;
			rotation.y = t.q.y;
			rotation.z = t.q.z;
			return *this;
		}

		std::string name;
		Vec3f position = { 0,0,0 };
		Vec3f scale = { 1,1,1 };
		Vec3f rotation = { 0,0,0 };
	};

	class Object  {
	public:
		Object() { };
		~Object() { };
		Transform transform = Transform();
	};

	struct Vertex {

		float x, y, z;
		float u, v;
		float nx, ny, nz;

		Vertex(float _x, float _y, float _z, float _u, float _v, float _nx, 
			float _ny, float _nz) :
			x(_x), y(_y), z(_z), u(_u), v(_v), nx(_nx), ny(_ny), nz(_nz){}

		Vertex() {
			x = y = z = 0.0f;
			u = v = 0.0f;
			nx = ny = nz = 0.0f;
		}
	};

	struct VertexUV {

		float x, y, z;
		float u, v;

		VertexUV(float _x, float _y, float _z, float _u, float _v) :
			x(_x), y(_y), z(_z), u(_u), v(_v){}

		VertexUV() {
			x = y = z = 0.0f;
			u = v = 0.0f;
		
		}
	};


	struct VertexRGBA {

		float x, y, z;
		float r, g, b, a;

		VertexRGBA(float _x, float _y, float _z, float _r, float _g, float _b, float _a) :
			x(_x), y(_y), z(_z), r(_r), g(_g), b(_b), a(_a) {}

		VertexRGBA() {
			x = y = z = 0.0f;
			r = g = b = a = 0.0f;
		}
	};
}