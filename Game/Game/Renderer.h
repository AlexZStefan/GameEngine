#pragma once
#include <glew.h>
#include <iostream>

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

	Vertex(){
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

	void draw();

	void destroy();

private:
	GLuint programID;
	GLint vertexPos2DLocation;
	GLuint vboTraiangle; 

	
};
}