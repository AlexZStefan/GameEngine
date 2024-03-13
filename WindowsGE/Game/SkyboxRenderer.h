#pragma once
#include "Camera.h"
#include "ShaderUtils.h"
#include "Functions.h"
#include "Structs.h"

#include <SDL_image.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <vector>
#include <string>

namespace GE {

	struct CubeVertex {
		float x, y, z;

		CubeVertex() {
			x = y = z = 0.0f;
		}
		CubeVertex(float _x, float _y, float _z) {
			x = _x;
			y = _y;
			z = _z;
		}


	};
	const float SIDE = 1.0f;

	const CubeVertex cube[] = {
		// Front face
		CubeVertex(-SIDE,  SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE, -SIDE),
		CubeVertex(SIDE, -SIDE, -SIDE),

		CubeVertex(SIDE, -SIDE, -SIDE),
		CubeVertex(SIDE,  SIDE, -SIDE),
		CubeVertex(-SIDE, SIDE, -SIDE),

		// Back face
		CubeVertex(-SIDE,  SIDE, SIDE),
		CubeVertex(-SIDE, -SIDE, SIDE),
		CubeVertex(SIDE, -SIDE, SIDE),

		CubeVertex(SIDE, -SIDE, SIDE),
		CubeVertex(SIDE,  SIDE, SIDE),
		CubeVertex(-SIDE, SIDE, SIDE),

		// Left face
		CubeVertex(-SIDE, -SIDE, SIDE),
		CubeVertex(-SIDE,  SIDE, SIDE),
		CubeVertex(-SIDE,  SIDE, -SIDE),

		CubeVertex(-SIDE,  SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE,  SIDE),

		// Right face
		CubeVertex(SIDE, -SIDE, SIDE),
		CubeVertex(SIDE,  SIDE, SIDE),
		CubeVertex(SIDE,  SIDE, -SIDE),

		CubeVertex(SIDE,  SIDE, -SIDE),
		CubeVertex(SIDE, -SIDE, -SIDE),
		CubeVertex(SIDE, -SIDE,  SIDE),

		// Top face
		CubeVertex(-SIDE, SIDE,  SIDE),
		CubeVertex(SIDE, SIDE,  SIDE),
		CubeVertex(SIDE, SIDE, -SIDE),

		CubeVertex(SIDE,  SIDE, -SIDE),
		CubeVertex(-SIDE, SIDE, -SIDE),
		CubeVertex(-SIDE, SIDE,  SIDE),

		// Bottom face
		CubeVertex(-SIDE, -SIDE,  SIDE),
		CubeVertex(SIDE, -SIDE,  SIDE),
		CubeVertex(SIDE, -SIDE, -SIDE),

		CubeVertex(SIDE, -SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE, -SIDE),
		CubeVertex(-SIDE, -SIDE,  SIDE),
	};

	class SkyboxRenderer : public Transform
	{
	public:
		//front, back, right, left, up, down
		SkyboxRenderer(std::string fname_front, std::string fname_back, std::string fname_right, std::string fname_left,
			std::string fname_up, std::string fname_down);
		~SkyboxRenderer();
	
	void draw(Shared<Camera> cam);
	void destroy();
	void bind();
	void release();

private:
	void createCubeMap(std::vector<std::string> filename);
	//void createCubeVbo();
	void createSkyboxProgram();
	
	void setAttribute();

	GLuint skyboxCubeMap;
	
	GLuint skyboxprogramId;
	GLint vertexPosLocation;
	GLuint vboSkybox;

	GLuint transformUniformId;
	GLuint viewUniformId;
	GLuint projectionUniformId;
	GLuint samplerId;
};

}
