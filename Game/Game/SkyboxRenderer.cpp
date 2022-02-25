#include "SkyboxRenderer.h"

//front, back, right, left, up, down
GE::SkyboxRenderer::SkyboxRenderer(std::string fname_front, std::string fname_back, std::string fname_right, std::string fname_left,
	std::string fname_up, std::string fname_down)
{
	std::vector<std::string> filenames;

	filenames.push_back(fname_front);
	filenames.push_back(fname_back);
	filenames.push_back(fname_right);
	filenames.push_back(fname_left);
	filenames.push_back(fname_up);
	filenames.push_back(fname_down);

	glGenBuffers(1, &vboSkybox);

	createCubeMap(filenames);
	createSkyboxProgram();
}

GE::SkyboxRenderer::~SkyboxRenderer()
{
}

void GE::SkyboxRenderer::createCubeMap(std::vector<std::string> filename)
{
	glGenTextures(1, &skyboxCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubeMap);

	for (int faceNum = 0; faceNum < 6; faceNum++) {
		SDL_Surface* surfaceImage = IMG_Load(filename[faceNum].c_str());

		if (surfaceImage == nullptr) {
			std::cerr << "Skybox image could not be loaded. Num:" << faceNum << "Could not be loaded" << std::endl;
			return;
		}

		GLenum format = surfaceImage->format->format;

		switch (format) {
		case SDL_PIXELFORMAT_RGBA32:
			format = GL_RGBA;
			break;
		case SDL_PIXELFORMAT_RGB24:
			format = GL_RGB;
			break;
		default:
			format = GL_RGB;
			break;
		}

		GLCALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceNum, 0, format, surfaceImage->w, surfaceImage->h, 0,
			format,
			GL_UNSIGNED_BYTE, surfaceImage->pixels));

		SDL_FreeSurface(surfaceImage);
	}
	GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCALL(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
}

void GE::SkyboxRenderer::createSkyboxProgram()
{
	const GLchar* V_ShaderCode[] = {
			"#version 140\n"
			"in vec3 vertexPos;\n"
			"uniform mat4 transform;\n"
			"out vec3 textCoord;\n"
			"uniform mat4 view;\n"
			"uniform mat4 projection;\n"
			"void main()\n"
			"{\n"
			"textCoord = vertexPos;\n"
			"vec4 v = vec4(vertexPos.xyz, 1);\n"
			"v = projection * view * transform * v;\n"
			"gl_Position = v;\n"
			"}\n"
	};

	// have to adjust if it`s only rgb text 
	const GLchar* F_ShaderCode[] = {
		"#version 140\n"
		"in vec3 textCoord;"
		"uniform samplerCube sampler;\n"
		"out vec4 fragmentColour;\n"
		"void main()\n"
		"{\n"
		"fragmentColour = vec4(texture(sampler, textCoord).rgb, 1.0f);\n"
		"}\n"
	};

	//// VERTEX SHADER INIT
	//GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//// Copy the source to gl, ready for compilation
	//glShaderSource(vertexShader, 1, V_ShaderCode, NULL);
	//// compile the code
	//GLCALL(glCompileShader(vertexShader));
	//// check for errors - presume shader did not compile
	//GLint isFShaderCompiledOK = GL_FALSE;
	//// get  compilation status from openGL
	//GLCALL(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isFShaderCompiledOK));
	//if (isFShaderCompiledOK != GL_TRUE) {
	//	std::cerr << "Unable to compile vertex shader" << std::endl;
	//	displayShaderCompilerError(vertexShader);
	//	return;
	//}

	//// FRAGMENT SHADER INIT 
	//GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//GLCALL(glShaderSource(fragmentShader, 1, F_ShaderCode, NULL));
	//GLCALL(glCompileShader(fragmentShader));
	//GLCALL(GLint isShaderCompiledOK = GL_FALSE);
	//GLCALL(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isShaderCompiledOK));
	//if (isShaderCompiledOK != GL_TRUE) {
	//	std::cerr << "Unable to compile the fragment shader" << std::endl;
	//	displayShaderCompilerError(fragmentShader);
	//	return;
	//}


	bool result = compileProgram(V_ShaderCode, F_ShaderCode, &skyboxprogramId);

	if (!result) {
		std::cerr << "Failed to create skybox program " << std::endl;

		return;
	}

	vertexLocation = glGetAttribLocation(skyboxprogramId, "vertexPos");

	if (vertexLocation == -1) {
		std::cerr << "Problem getting vertexPosition" << std::endl;
	}

	viewUniformId = glGetUniformLocation(skyboxprogramId, "view");
	projectionUniformId = glGetUniformLocation(skyboxprogramId, "projection");
	transformUniformId = glGetUniformLocation(skyboxprogramId, "transform");
	samplerId = glGetUniformLocation(skyboxprogramId, "sampler");
}

void GE::SkyboxRenderer::draw(Camera* cam)
{
	bool isDepthTestEnable = glIsEnabled(GL_DEPTH_TEST);

	glDisable(GL_DEPTH_TEST);

	glm::mat4 camView = cam->getViewMatrix();
	glm::mat4 projection = cam->getProjectionMatrix();

	camView[3][0] = 0.0f;
	camView[3][1] = 0.0f;
	camView[3][2] = 0.0f;

	GLCALL(glUniformMatrix4fv(viewUniformId, 1, GL_FALSE, glm::value_ptr(camView)));
	GLCALL(glUniformMatrix4fv(projectionUniformId, 1, GL_FALSE, glm::value_ptr(projection)));
	
	bind();

	setAttribute();

	GLCALL(glActiveTexture(GL_TEXTURE0));
	GLCALL(glUniform1i(samplerId, 0));
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxCubeMap);

	glDrawArrays(GL_TRIANGLES, 0, sizeof(cube) / sizeof(skyboxCubeMap));

	GLCALL(glDisableVertexAttribArray(vertexLocation));

	release();

	if (isDepthTestEnable) {
		glEnable(GL_DEPTH_TEST);
	}
}
void GE::SkyboxRenderer::setAttribute()
{
	GLCALL(glEnableVertexAttribArray(vertexLocation));

	GLCALL(glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)offsetof(CubeVertex, x)));
}

void GE::SkyboxRenderer::bind()
{
	//std::cout << sizeof(cube) << " sizeof vert" << sizeof(cube)/ sizeof(CubeVertex) << std::endl;
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, vboSkybox));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW));
}

void GE::SkyboxRenderer::release()
{
	GLCALL(glDisableVertexAttribArray(vertexLocation));
	GLCALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCALL(glUseProgram(0));

}


void GE::SkyboxRenderer::destroy()
{
	GLCALL(glDeleteProgram(skyboxprogramId));
	GLCALL(glDeleteBuffers(1, &vboSkybox));
	GLCALL(glDeleteTextures(1, &skyboxCubeMap));
}
