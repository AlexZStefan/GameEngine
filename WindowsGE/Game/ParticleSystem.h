#pragma once
#include "Functions.h"
#include "Structs.h"
#include "BillboardRenderer.h"
#include "Shader.h"
#include <list>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <glm/glm.hpp>


class Shader;
namespace GE {


	class ParticleRenderer : public BillboardRenderer
	{
		Vec3f billboardT[6] = {
			Vec3f(1.f, 1.0f, 1.0f),
			Vec3f(1.f, 1.0f, 1.0f),
			Vec3f(1.f, 1.0f, 1.0f),
			Vec3f(1.f, 1.0f, 1.0f),
			Vec3f(1.f, 1.0f, 1.0f),
			Vec3f(1.f, 1.0f, 1.0f)
		};

	public:
		using BillboardRenderer::BillboardRenderer;

		ParticleRenderer(Shared<Billboard> b, Shared<Camera> cam, Vec3f scale, int spawnRate,unsigned int _amount = 1) : iAmount(_amount),
			BillboardRenderer("particles.vert", "billboard.frag") {

			game_camera = cam;
			modelMatrices = new glm::mat4[iAmount];
			particle_idx = _amount - 1;
			nextParticleSpawn = spawnRate;
			lastParticleSpawn = SDL_GetTicks();

			for (int i = 0; i < iAmount; i++)
			{
				// position rot and scale
				particles.push_back(Particle(Vec3f(0, -50, 0), Vec3f(1, 1, 1), scale));

				// transformation matrix; 
				glm::mat4 transformationMat = glm::mat4(1.0f);

				transformationMat = glm::translate(transformationMat,
					glm::vec3(b->transform.position.x + (i * 10), b->transform.position.y + (i * 10), b->transform.position.z + (i * 10)));
				transformationMat = glm::rotate(transformationMat,
					glm::radians(b->transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
				transformationMat = glm::rotate(transformationMat,
					glm::radians(b->transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
				transformationMat = glm::rotate(transformationMat,
					glm::radians(b->transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
				transformationMat = glm::scale(transformationMat,
					glm::vec3(1, 1, 0));

				instanceMatrix.push_back(transformationMat);
			}
		};

		glm::mat4 transformUpdate(float _x, float _y, float _z, float _scaleX, float _scaleY, float _scaleZ, float _rotX, float _rotY, float _rotZ) {
			glm::mat4 transformationMat = glm::mat4(1.0f);

			transformationMat = glm::translate(transformationMat,
				glm::vec3(_x, _y, _z));
			transformationMat = glm::rotate(transformationMat,
				glm::radians(_rotX), glm::vec3(1.0f, 0.0f, 0.0f));
			transformationMat = glm::rotate(transformationMat,
				glm::radians(_rotY), glm::vec3(0.0f, 1.0f, 0.0f));
			transformationMat = glm::rotate(transformationMat,
				glm::radians(_rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
			transformationMat = glm::scale(transformationMat,
				glm::vec3(_scaleX, _scaleY, _scaleZ));

			return transformationMat;
		};

		void Update(Shared<Camera> cam, Vec3f to, Vec3f from, Vec3f speed);

		virtual void draw(Shared<Billboard>Billboard, Shared<Camera> cam)  override;
		~ParticleRenderer() { SDL_FreeSurface(particleSurface); };
		virtual void loadVA() override;

		void bindAll();

		void generateTransforms();

	private:

		struct Particle {
			Particle(Vec3f _pos, Vec3f _rot, Vec3f _scale) {
				transform.position = _pos;
				transform.rotation = _rot;
				transform.scale = _scale;
			}

			bool active = false;
			Transform transform;
			float lifeTime = 10.0f;
			float spawnTime; 

			float velocity = 1.0f;
		};

		SDL_Surface* particleSurface;
		std::vector<Particle> particles;
		Shared<Camera> game_camera;
		Texture* texture;

		// instancing
		std::vector<glm::mat4>mats;
		std::vector<glm::mat4> instanceMatrix;

		GLuint vboIDInstance;
		glm::mat4* modelMatrices;

		unsigned int iAmount;

		Shared<VertexBuffer> vbI;
		Shared<VertexBufferLayout> vlI;
		GLuint mat4Transform;

		Uint32 emissionRate = 1;
		Uint32 emmisionRange = 90;
		Uint32 Gravity = 10;

		Uint32 lastParticleSpawn;
		Uint32 nextParticleSpawn;
		int particle_idx; 

	};


}