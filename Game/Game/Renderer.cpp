#include "Renderer.h"

namespace GE {

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

	Renderer::Renderer(std::shared_ptr<Model> _model) : model(_model)
	{
		pos_x = pos_y = pos_z = 0.0f;
		rot_x = rot_y = rot_z = 0.0f;
		scale_x = 1.0f;
		scale_y = 1.0f;
		scale_z = 1.0f;

		programId = 0;
		vertexPosLocation = 0;
		vertexUVLocation = 0;

		transformUniformId = 0;
		viewUniformId = 0;
		projectionUniformId = 0;
		samplerId = 0;

		init();
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::init()
	{
		std::string v_shader_source = loadShaderSourceCode("basic_vshader.vert");
		std::string f_shader_source = loadShaderSourceCode("basic_fshader.frag");

		// Due to the unique way OpenGL handles shader source, OpenGL expects
		// an array of strings.  In this case, create an array of the
		// loaded source code strings and pass to compileProgram for compilation
		const GLchar* v_source_array[] = { v_shader_source.c_str() };
		const GLchar* f_source_array[] = { f_shader_source.c_str() };

		bool result = compileProgram(v_source_array, f_source_array, &programId);

		if (!result) {
			std::cerr << "Failed to create skybox program " << std::endl;

			return;
		}

		vertexPosLocation = glGetAttribLocation(programId, "vertexPos");
		if (vertexPosLocation == -1) {
			std::cerr << "Problem getting vertexPos" << std::endl;
		}

		vertexUVLocation = glGetAttribLocation(programId, "vi_UV");
		if (vertexUVLocation == -1) {
			std::cerr << "Problem getting vertex UV" << std::endl;
		}

		transformUniformId = glGetUniformLocation(programId, "transform");
		projectionUniformId = glGetUniformLocation(programId, "projection");
		viewUniformId = glGetUniformLocation(programId, "view");
		samplerId = glGetUniformLocation(programId, "sampler");
	}

	void Renderer::update()
	{
	}   

	void Renderer::draw(Camera* cam)
	{
		// gets disabled at end of F()
		// stops renderign faces not visible to the camera
		glEnable(GL_CULL_FACE);

		GLCALL(glUseProgram(programId));	

		GLCALL(glEnableVertexAttribArray(vertexPosLocation));
		GLCALL(glEnableVertexAttribArray(vertexUVLocation));
		//GLCALL(glUniform3f(vertexPosLocation, 500.0f, 100.0f, 1.0f));

		glm::mat4 transformationMat = glm::mat4(1.0f);
		transformationMat = glm::translate(transformationMat, glm::vec3(pos_x, pos_y, pos_z));
		transformationMat = glm::rotate(transformationMat, glm::radians(rot_x), glm::vec3(1.0f, 0.0f, 0.0f));
		transformationMat = glm::rotate(transformationMat, glm::radians(rot_y), glm::vec3(0.0f, 1.0f, 0.0f));
		transformationMat = glm::rotate(transformationMat, glm::radians(rot_z), glm::vec3(0.0f, 0.0f, 1.0f));
		transformationMat = glm::scale(transformationMat, glm::vec3(scale_x, scale_y, scale_z));

		glm::mat4 viewMat = cam->getViewMatrix();
		glm::mat4 projectionMat = cam->getProjectionMatrix();

		// functions transfer ram to graphics ram 
		GLCALL(glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(transformationMat)));
		GLCALL(glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat)));
		GLCALL(glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat)));

			model->bindTexture(samplerId);
			model->bindVBO();
			if (model->getIndicesCount() > 0)
			{
			// draw indices 
				model->bindIBO();
				model->setAttribute(vertexPosLocation, 3, vertexUVLocation, 2);	
				GLCALL(glDrawElements(GL_TRIANGLES, model->getIndicesCount(), GL_UNSIGNED_INT, nullptr))
				model->unbindIBO();
			}
			else
			{
			// draw vertex
				model->setAttribute(vertexPosLocation, 3, vertexUVLocation, 2);	
				GLCALL(glDrawArrays(GL_TRIANGLES, 0, model->getNumVertices()));
			}
			model->unbindVBO();
			model->unbindTexture();

		GLCALL(glDisableVertexAttribArray(vertexPosLocation));
		GLCALL(glDisableVertexAttribArray(vertexUVLocation));
		GLCALL(glUseProgram(0));
		glDisable(GL_CULL_FACE);
	}

	void Renderer::destroy()
	{
		// Release object allocated for program and vertex buffer obj 
		GLCALL(glDeleteProgram(programId));
		GLCALL(glDeleteBuffers(1, &vboTraiangle));
	}
}