#include "Renderer.h"

namespace GE {
	extern Coordinator gCoordinator;

	VertexRGBA triangleVertecies[] = {
		VertexRGBA(-.50f, .0f, .0f, 1.0f, 1.0f, 1.0f, 1.0f),
		VertexRGBA(.50f, .0f, .0f, 1.0f, 0.0f, 0.0f, 0.0f),
		VertexRGBA(.0f, -1.0f, .0f, 1.0f, 0.0f, 0.0f, 1.0f),
	};

	GLfloat vertexData[] = {
		-1.f, .0f, .0f,
		1.f, .0f, .0f,
		.0f, 1.f, .0f
	};

	Renderer::Renderer()
	{
		vertexPosLocation = -1;
		vertexUVLocation = -1;
		transformUniformId = -1;
		viewUniformId = -1;
		projectionUniformId = -1;
		samplerId = -1;
		/*m_ShaderID = _shader->getProgramId();
		vertexPosLocation = 0;
		vertexUVLocation = 0;
		transformUniformId = 0;
		viewUniformId = 0;
		projectionUniformId = 0;
		samplerId = 0;*/
		init();
	}

	/*Renderer::Renderer(Shared<Shader> _shader)
	{
		m_ShaderID = _shader->getProgramId();

		vertexPosLocation = 0;
		vertexUVLocation = 0;

		transformUniformId = 0;
		viewUniformId = 0;
		projectionUniformId = 0;
		samplerId = 0;
	}*/

	Renderer::~Renderer()
	{
		// Release object allocated for program and vertex buffer obj 
	}


	void Renderer::init()
	{
		/*m_ShaderID = shader->getProgramId();

		transformUniformId = glGetUniformLocation(m_ShaderID, "transform");
		projectionUniformId = glGetUniformLocation(m_ShaderID, "projection");
		viewUniformId = glGetUniformLocation(m_ShaderID, "view");
		samplerId = glGetUniformLocation(m_ShaderID, "textureSampler");
		normalSamplerId = glGetUniformLocation(m_ShaderID, "normal_map");

		model->vb = std::make_shared<VertexBuffer>
			(model->getNumVertices() * sizeof(Vertex), model->vertices.data());
		model->vl = std::make_shared<VertexBufferLayout>();
		model->vl->push<float>(3);
		model->vl->push<float>(2);
		if (model->hasNormals()) {
			model->vl->push<float>(3);
		}

		model->va = std::make_shared<VertexArray>();
		model->va->addBuffer(model->vb, model->vl,0);

		model->va->unbind();
		model->vb->unbind();*/
	}

	void Renderer::update(Shared<Camera> cam)
	{
		/*std::vector<Model> entitiesModel;

		for (auto m : mEntities) {
			auto& mod = gCoordinator.GetComponent<Model>(m);
			entitiesModel.push_back(mod);
		}*/

		for (auto const& entity : mEntities)
		{
			//std::cout << " add data to the renderer" << entity << "\n";
			auto& transform = gCoordinator.GetComponent<Transform>(entity);
			auto& model = gCoordinator.GetComponent<Model>(entity);
#pragma region Old

			//mod.shader->bind();
			//transformUniformId = glGetUniformLocation(mod.m_ShaderID, "transform");
			//projectionUniformId = glGetUniformLocation(mod.m_ShaderID, "projection");
			//viewUniformId = glGetUniformLocation(mod.m_ShaderID, "view");
			//samplerId = glGetUniformLocation(mod.m_ShaderID, "textureSampler");
			//normalSamplerId = glGetUniformLocation(mod.m_ShaderID, "normal_map");
			//
			//glEnable(GL_CULL_FACE);
			//
			//glm::mat4 transformationMat = glm::mat4(1.0f);
			//transformationMat = glm::translate(transformationMat,
			//	glm::vec3(mod.transform.position.x, mod.transform.position.y, mod.transform.position.z));
			//transformationMat = glm::rotate(transformationMat,
			//	glm::radians(mod.transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			//transformationMat = glm::rotate(transformationMat,
			//	glm::radians(mod.transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			//transformationMat = glm::rotate(transformationMat,
			//	glm::radians(mod.transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			//transformationMat = glm::scale(transformationMat,
			//	glm::vec3(mod.transform.scale.x, mod.transform.scale.y, mod.transform.scale.z));

			//glm::mat4 viewMat = cam->getViewMatrix();
			//glm::mat4 projectionMat = cam->getProjectionMatrix();

			//GLCALL(glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(transformationMat)));
			//GLCALL(glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat)));
			//GLCALL(glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat)));

			//// bind texture - bind VBO or IBO - set reading attributes (pos, uv, normals) - draw elements
			//mod.va->bind();

			///*glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, mod.getMaterial());
			//glActiveTexture(GL_TEXTURE2);
			//glBindTexture(GL_TEXTURE_2D, mod.getNormalMap());*/

			//mod.bindTexture(samplerId);
			//if (mod.hasNormalMap()) {
			//	mod.bindNormalMap(mod.getNormalMap());
			//	glActiveTexture(GL_TEXTURE0);
			//	glActiveTexture(GL_TEXTURE1);
			//	glUniform1i(glGetUniformLocation(mod.m_ShaderID, "normal_map"), 2);
			//	glActiveTexture(GL_TEXTURE2);
			//	glBindTexture(GL_TEXTURE_2D, mod.getNormalMap());
			//}

			//if (mod.getIndicesCount() > 0)
			//{ // draw indices 
			//	mod.bindIBO();
			//	GLCALL(glDrawElements(GL_TRIANGLES, mod.getIndicesCount(), GL_UNSIGNED_INT, nullptr))
			//		mod.unbindIBO();
			//}
			//else
			//{// draw vertex
			//	GLCALL(glDrawArrays(GL_TRIANGLES, 0, mod.getNumVertices()));
			//}
			//////set transform matrix - issue drawcall after this to create multiple objects
			////glm::vec3 translateB = { model->position.x + 10,10,10 };
			////glm::mat4 trnn = glm::translate(glm::mat4(1.0f), translateB);
			////shader->SetUniformMat4f("transform", trnn);

			// mod.va->unbind();
			// mod.unbindTexture();
			// GLCALL(glUseProgram(0));
			// glDisable(GL_CULL_FACE);
#pragma endregion
			glEnable(GL_CULL_FACE);
			GLCALL(glUseProgram(model.m_ShaderID));

			transformUniformId = glGetUniformLocation(model.m_ShaderID, "transform");
			projectionUniformId = glGetUniformLocation(model.m_ShaderID, "projection");
			viewUniformId = glGetUniformLocation(model.m_ShaderID, "view");
			samplerId = glGetUniformLocation(model.m_ShaderID, "textureSampler");
			normalSamplerId = glGetUniformLocation(model.m_ShaderID, "normal_map");

			glm::mat4 transformationMat = glm::mat4(1.0f);
			transformationMat = glm::translate(transformationMat,
				glm::vec3(transform.position.x, transform.position.y, transform.position.z));
			transformationMat = glm::rotate(transformationMat,
				glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			transformationMat = glm::rotate(transformationMat,
				glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			transformationMat = glm::rotate(transformationMat,
				glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
			transformationMat = glm::scale(transformationMat,
				glm::vec3(transform.scale.x, transform.scale.y, transform.scale.z));

			glm::mat4 viewMat = cam->getViewMatrix();
			glm::mat4 projectionMat = cam->getProjectionMatrix();

			GLCALL(glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(transformationMat)));
			GLCALL(glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat)));
			GLCALL(glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat)));

			// bind texture - bind VBO or IBO - set reading attributes (pos, uv, normals) - draw elements
			model.BindVA();
			//glUniform1i(glGetUniformLocation(model.m_ShaderID, "textureSampler"),0);
			/*glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, model.getMaterial());
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, model.getNormalMap());*/

			model.bindTexture(samplerId);
			if (model.hasNormals()) {
				model.bindNormalMap();
			//	glUniform1i(glGetUniformLocation(model.m_ShaderID, "normal_map"), 2);
				/*glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, model.getNormalMap());*/
			}
						
			// std::cout << "render model indices - " + renderIndeces;
			
			if (false && model.name == "ter" &&  model.getIndicesCount() > 0) // remove false
			{ // draw indices 
				model.bindIBO();
				GLCALL(glDrawElements(GL_TRIANGLES, model.getIndicesCount(), GL_UNSIGNED_INT, nullptr))
					model.unbindIBO();
			}
			else
			{// draw vertex
				GLCALL(glDrawArrays(GL_TRIANGLES, 0, model.getNumVertices()));
			}
			////set transform matrix - issue drawcall after this to create multiple objects
			//glm::vec3 translateB = { model.position.x + 10,10,10 };
			//glm::mat4 trnn = glm::translate(glm::mat4(1.0f), translateB);
			//shader->SetUniformMat4f("transform", trnn);

			model.UnbindVA();
			model.unbindTexture();

			GLCALL(glUseProgram(0));
			glDisable(GL_CULL_FACE);
		}
	}

	void Renderer::draw(Shared<Camera> cam)
	{
		//// gets disabled at end of F()
		//// stops renderign faces not visible to the camera
		//glEnable(GL_CULL_FACE);

		//GLCALL(glUseProgram(m_ShaderID));

		//glm::mat4 transformationMat = glm::mat4(1.0f);
		//transformationMat = glm::translate(transformationMat,
		//	glm::vec3(model->transform.position.x, model->transform.position.y, model->transform.position.z));
		//transformationMat = glm::rotate(transformationMat,
		//	glm::radians(model->transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		//transformationMat = glm::rotate(transformationMat,
		//	glm::radians(model->transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		//transformationMat = glm::rotate(transformationMat,
		//	glm::radians(model->transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		//transformationMat = glm::scale(transformationMat,
		//	glm::vec3(model->transform.scale.x, model->transform.scale.y, model->transform.scale.z));


		//glm::mat4 viewMat = cam->getViewMatrix();
		//glm::mat4 projectionMat = cam->getProjectionMatrix();

		//GLCALL(glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(transformationMat)));
		//GLCALL(glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat)));
		//GLCALL(glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat)));

		//// bind texture - bind VBO or IBO - set reading attributes (pos, uv, normals) - draw elements
		//model->va->bind();

		////glUniform1i(glGetUniformLocation(m_ShaderID, "textureSampler"),0);
		///*glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, model->getMaterial());
		//glActiveTexture(GL_TEXTURE2);
		//glBindTexture(GL_TEXTURE_2D, model->getNormalMap());*/

		//model->bindTexture(samplerId);
		//if (model->hasNormalMap()) {
		//	//model->bindNormalMap(model->getNormalMap());
		//	glUniform1i(glGetUniformLocation(m_ShaderID, "normal_map"), 2);
		//	glActiveTexture(GL_TEXTURE2);
		//	glBindTexture(GL_TEXTURE_2D, model->getNormalMap());
		//}

		//if (model->getIndicesCount() > 0)
		//{ // draw indices 
		//	model->bindIBO();
		//	GLCALL(glDrawElements(GL_TRIANGLES, model->getIndicesCount(), GL_UNSIGNED_INT, nullptr))
		//		model->unbindIBO();
		//}
		//else
		//{// draw vertex
		//	GLCALL(glDrawArrays(GL_TRIANGLES, 0, model->getNumVertices()));
		//}
		//////set transform matrix - issue drawcall after this to create multiple objects
		////glm::vec3 translateB = { model->position.x + 10,10,10 };
		////glm::mat4 trnn = glm::translate(glm::mat4(1.0f), translateB);
		////shader->SetUniformMat4f("transform", trnn);

		//model->va->unbind();
		//model->unbindTexture();

		//GLCALL(glUseProgram(0));
		//glDisable(GL_CULL_FACE);

	}
}
