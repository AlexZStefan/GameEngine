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

		programID = 0;
		vertexPosLocation = 0;
		vertexUVLocation = 0;

		transformUniformId = 0;
		viewUniformId = 0;
		projectionUniformId = 0;
		sampleId = 0;

		init();
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::init()
	{
		/*ShaderSource source = ParseShader("Basic.shader");
		std::cerr << source.vertexSource << std::endl;
		std::cerr << source.fragmentSource << std::endl;*/

		const GLchar* V_ShaderCode[] = {
			"#version 140\n"
			"in vec3 vertexPos;\n"
			"in vec2 vi_UV;\n"
			"out vec2 vo_UV;\n"
			"uniform mat4 transform;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n"
			"void main()\n"
			"{\n"
			"vo_UV = vi_UV;\n"
			"vec4 v = vec4(vertexPos.xyz, 1);\n"
			"v = projection * view * transform * v;\n"
			//"gl_Position = vec4(vertexPos.x, vertexPos.y, 0,1);\n"
			"gl_Position = v;\n"
			"}\n"
		};

		// have to adjust if it`s only rgb text 
		const GLchar* F_ShaderCode[] = {
			"#version 140\n"
			"in vec2 vo_UV;"
			"uniform sampler2D sampler;\n"
			"out vec4 fragmentColour;\n"
			"void main()\n"
			"{\n"
			"fragmentColour = texture(sampler, vo_UV).rgba;\n"
			"}\n"
		};

		bool result = compileProgram(V_ShaderCode, F_ShaderCode, &programID);

		if (!result) {
			std::cerr << "Failed to create skybox program " << std::endl;

			return;
		}


		vertexPosLocation = glGetAttribLocation(programID, "vertexPos");
		if (vertexPosLocation == -1) {
			std::cerr << "Problem getting vertexPos" << std::endl;
		}

		vertexUVLocation = glGetAttribLocation(programID, "vi_UV");
		if (vertexUVLocation == -1) {
			std::cerr << "Problem getting vertex UV" << std::endl;
		}

		transformUniformId = glGetUniformLocation(programID, "transform");
		projectionUniformId = glGetUniformLocation(programID, "projection");
		viewUniformId = glGetUniformLocation(programID, "view");
		sampleId = glGetUniformLocation(programID, "sampler");
	}

	void Renderer::update()
	{
	}   

	void Renderer::draw(Camera* cam)
	{
		// gets disabled at end of F()
		// stops renderign faces not visible to the camera
		glEnable(GL_CULL_FACE);

		GLCALL(glUseProgram(programID));	

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

			model->bindTexture(sampleId);
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
		GLCALL(glDeleteProgram(programID));
		GLCALL(glDeleteBuffers(1, &vboTraiangle));
	}
}