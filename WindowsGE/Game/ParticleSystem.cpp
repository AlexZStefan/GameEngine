#include "ParticleSystem.h"

namespace GE {
	float arr = 0;
	void ParticleRenderer::Update(Shared<Camera> cam, Vec3f to, Vec3f from, Vec3f speed)
	{

		game_camera = cam;

		if (SDL_GetTicks() - lastParticleSpawn >= nextParticleSpawn)
		{
			float cam_x = game_camera->getPosX() + (rand() % 200 - 100);
			float cam_y = game_camera->getPosY() + 20;
			float cam_z = game_camera->getPosZ() + (rand() % 200 - 100);
			

			// set intial position if innactive
			if (particles[particle_idx].active == false)
			{
				particles[particle_idx].transform.position =
					Vec3f(0, -50, 0);
			};

			lastParticleSpawn = SDL_GetTicks();

			particles[particle_idx].active = true;
			particles[particle_idx].transform.position = Vec3f(from.x, from.y, from.z);

			particle_idx--;
			if (particle_idx <= 0) particle_idx = iAmount - 1;
		}

		for (auto& p : particles) {
			if (p.active == true) {
				p.transform.position.x += speed.x ;
				p.transform.position.z += speed.z ;
				p.transform.position.y += speed.y ;
			}
		}
		//particlePostions.remove_if([](SDL_Rect rct) { return rct.y >= 500; });
	}

	void ParticleRenderer::draw(Shared<Billboard>b, Shared<Camera> cam)
	{
		arr += 0.01;
		// activate shader
		glUseProgram(shaderID);

		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		va->bind();


		glBindBuffer(GL_ARRAY_BUFFER, vboIDInstance);

		GLint matLoc = glGetAttribLocation(shaderID, "instanceMatrix");

		GLCALL(glEnableVertexAttribArray(matLoc));
		GLCALL(glEnableVertexAttribArray(matLoc + 1));
		GLCALL(glEnableVertexAttribArray(matLoc + 2));
		GLCALL(glEnableVertexAttribArray(matLoc + 3));

		// update matrix 
		for (int i = 0; i < iAmount; i++)
		{
			Vec3f particle_position = particles[i].transform.position;
			Vec3f particle_scale = particles[i].transform.scale;

			glm::mat4 transformationMat = glm::mat4(1.0f);

			transformationMat = glm::translate(transformationMat,
				glm::vec3(particle_position.x, particle_position.y, particle_position.z));
			transformationMat = glm::rotate(transformationMat,
				glm::radians(360.0f - cam->getYaw() - 90.0f),
				glm::vec3(0.0f, 1.0f, 0.0f));

			transformationMat = glm::scale(transformationMat,
				glm::vec3(particle_scale.x, particle_scale.z, 0));

			instanceMatrix[i] = transformationMat;
		}

		glBufferData(GL_ARRAY_BUFFER, instanceMatrix.size() * sizeof(glm::mat4), instanceMatrix.data(), GL_DYNAMIC_DRAW);

		GLCALL(glVertexAttribPointer(matLoc, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(0)));
		GLCALL(glVertexAttribPointer(matLoc + 1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 1)));
		GLCALL(glVertexAttribPointer(matLoc + 2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 2)));
		GLCALL(glVertexAttribPointer(matLoc + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 3)));

		glVertexAttribDivisor(matLoc, 1);
		glVertexAttribDivisor(matLoc + 1, 1);
		glVertexAttribDivisor(matLoc + 2, 1);
		glVertexAttribDivisor(matLoc + 3, 1);

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(samplerId, 0);
		glBindTexture(GL_TEXTURE_2D, b->getTexture()->getTexture());
		glActiveTexture(0);

		//ib->bind();
		glm::mat4 viewMat = cam->getViewMatrix();
		glm::mat4 projectionMat = cam->getProjectionMatrix();

		GLCALL(glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat)));
		GLCALL(glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat)));

		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, iAmount);

		//glDrawElementsInstanced(GL_TRIANGLES, 6, GL_Unsigned)FLOAT, indices, iAmount);

		//ib->unbind();
		va->unbind();

		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glUseProgram(0);
	}

	void ParticleRenderer::loadVA()
	{
		// Create the vertex buffer for the quad
		// elements to draw * vertex size amount * type size
		vb = std::make_shared<VertexBuffer>(6 * 5 * sizeof(float), &billboard);
		vl = std::make_shared<VertexBufferLayout>();
		vl->push<float>(3);
		vl->push<float>(2);
		ib = std::make_shared<IndexBuffer>(6, &indices);
		va = std::make_shared<VertexArray>();
		va->addBuffer(vb, vl, 0);
		vb->unbind();
		va->bind();

		glUseProgram(shaderID);

		glGenBuffers(1, &vboIDInstance);
		glBindBuffer(GL_ARRAY_BUFFER, vboIDInstance);
		glBufferData(GL_ARRAY_BUFFER, instanceMatrix.size() * sizeof(glm::mat4), instanceMatrix.data(), GL_STATIC_DRAW);

		GLint matLoc = glGetAttribLocation(shaderID, "instanceMatrix");

		GLCALL(glEnableVertexAttribArray(matLoc));
		GLCALL(glEnableVertexAttribArray(matLoc + 1));
		GLCALL(glEnableVertexAttribArray(matLoc + 2));
		GLCALL(glEnableVertexAttribArray(matLoc + 3));

		GLCALL(glVertexAttribPointer(matLoc, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(0)));
		GLCALL(glVertexAttribPointer(matLoc + 1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 1)));
		GLCALL(glVertexAttribPointer(matLoc + 2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 2)));
		GLCALL(glVertexAttribPointer(matLoc + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 3)));

		glVertexAttribDivisor(matLoc, 1);
		glVertexAttribDivisor(matLoc + 1, 1);
		glVertexAttribDivisor(matLoc + 2, 1);
		glVertexAttribDivisor(matLoc + 3, 1);

		//vbI->unbind();
		ib->unbind();
		va->unbind();

		GLCALL(glDisableVertexAttribArray(matLoc));
		GLCALL(glDisableVertexAttribArray(matLoc + 1));
		GLCALL(glDisableVertexAttribArray(matLoc + 2));
		GLCALL(glDisableVertexAttribArray(matLoc + 3));
		glUseProgram(0);
	}

	void ParticleRenderer::generateTransforms()
	{
	}



}