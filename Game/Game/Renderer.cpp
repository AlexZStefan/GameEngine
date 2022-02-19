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

	void displayShaderCompilerError(GLuint shaderId) {
		GLint MsgLen = 0;

		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &MsgLen);

		if (MsgLen > 1) {
			//GLchar* Msg = new GLchar[MsgLen + 1];

			GLchar* Msg = (GLchar*)_malloca(MsgLen * sizeof(GLchar));

			glGetShaderInfoLog(shaderId, MsgLen, NULL, Msg);

			std::cerr << "Error compiling shader\n" << Msg << std::endl;

			//delete[] Msg;
			glDeleteShader(shaderId);
		}
	}

	Renderer::Renderer()
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

		model = std::make_unique<Model>();
		
		bool result = model->loadFromFile("./resources/assets/models/ship.obj", true);
		
		texture = std::make_unique<Texture>("./resources/assets/models/ship_uv.jpg");
		text2 = new Texture("./resources/assets/textures/terrain-texture.png");

		model->setMaterial(texture->getTexture());

		if (!result) {
			std::cerr << "failed to load model" << std::endl;
		}
		
		terrain = std::make_unique<TerrainGenerator>("./resources/assets/textures/hmap.jpg",1,1);
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

		// VERTEX SHADER INIT
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		// Copy the source to gl, ready for compilation
		glShaderSource(vertexShader, 1, V_ShaderCode, NULL);
		// compile the code
		GLCALL(glCompileShader(vertexShader));
		// check for errors - presume shader did not compile
		GLint isFShaderCompiledOK = GL_FALSE;
		// get  compilation status from openGL
		GLCALL(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isFShaderCompiledOK));
		if (isFShaderCompiledOK != GL_TRUE) {
			std::cerr << "Unable to compile vertex shader" << std::endl;
			displayShaderCompilerError(vertexShader);
			return;
		}

		// FRAGMENT SHADER INIT 
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		GLCALL(glShaderSource(fragmentShader, 1, F_ShaderCode, NULL));
		GLCALL(glCompileShader(fragmentShader));
		GLCALL(GLint isShaderCompiledOK = GL_FALSE);
		GLCALL(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isShaderCompiledOK));
		if (isShaderCompiledOK != GL_TRUE) {
			std::cerr << "Unable to compile the fragment shader" << std::endl;
			displayShaderCompilerError(fragmentShader);
			return;
		}

		// create program and store it`s ID 
		programID = glCreateProgram();
		// attach shader to program obj 
		GLCALL(glAttachShader(programID, vertexShader));
		GLCALL(glAttachShader(programID, fragmentShader));
		// link program to create executable program and use to render objects
		// this will exists in graphics memory 

		GLCALL(glLinkProgram(programID));
		GLint isProgramLinked = GL_FALSE;
		// shader error check 
		GLCALL(glGetProgramiv(programID, GL_LINK_STATUS, &isProgramLinked));
		if (isProgramLinked != GL_TRUE) {
			std::cerr << "Failed to link program" << std::endl;
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

	int asdas = 100;

	void Renderer::draw(Camera* cam)
	{
		asdas--; 
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

		//drawLine(glm::vec3(0, 0, 0), glm::vec3(100, 0, 0));

		// functions transfer ram to graphics ram 
		GLCALL(glUniformMatrix4fv(transformUniformId, 1, GL_FALSE, glm::value_ptr(transformationMat)));
		GLCALL(glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(viewMat)));
		GLCALL(glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projectionMat)));

		// select texture
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(sampleId, 0);
		glBindTexture(GL_TEXTURE_2D, text2->getTexture());

		terrain->bindIbo();

		GLCALL(glVertexAttribPointer(vertexPosLocation, 3,
			GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x)));

		GLCALL(glVertexAttribPointer(vertexUVLocation, 2,
			GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u)));

		GLCALL(glDrawElements(GL_TRIANGLES, terrain->indexCount, GL_UNSIGNED_INT, nullptr));
		terrain->unbindIbo();
		glBindTexture(GL_TEXTURE_2D, 0);

		// select texture
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(sampleId, 0);
		glBindTexture(GL_TEXTURE_2D, texture->getTexture());

		if (asdas > 1)
		{
		model->bindVBO();
		GLCALL(glVertexAttribPointer(vertexPosLocation, 3,
			GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x)));

		GLCALL(glVertexAttribPointer(vertexUVLocation, 2,
			GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u)));
		
		GLCALL(glDrawArrays(GL_TRIANGLES, 0, model->getNumVertices()));
		model->unbindVBO();
		glBindTexture(GL_TEXTURE_2D, 0);
		}

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