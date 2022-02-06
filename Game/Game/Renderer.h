#pragma once
#include "Camera.h"
#include <glew.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// take out if not req 
#include <SDL.h>


namespace GE {

	//const GLchar* V_ShaderCode[] = {
	//	"#version140\n"
	//	"in vec3 vertexPos3D;\n"
	//	"uniform mat4 transform;\n"
	//	"uniform mat4 view;\n"
	//	"uniform mat4 projection;\n"
	//	"void main(){\n"
	//	"vec4 v = vec4(vertexPos3D.xyz, 1);\n"
	//	"v= projection * view * transform & v;\n"
	//	"gl_Position = v;"
	//	"}\n;"
	//};

	struct Vertex {

		float x, y, z;

		Vertex(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

		Vertex() {
			x = y = z = 0.0f;
		}
	};

	class Renderer
	{
	public:
		Renderer();
		virtual ~Renderer();

		void init();

		void update();

		void draw(Camera* cam);

		void destroy();

	private:
		GLuint programID;
		GLint vertexPos2DLocation;
		GLuint vboTraiangle;

		float pos_x, pos_y, pos_z;
		float rot_x, rot_y, rot_z;
		float scale_x, scale_y, scale_z;

		GLuint transformUniformId;
		GLuint viewUniformId;
		GLuint projectionUniformId;


	};
}