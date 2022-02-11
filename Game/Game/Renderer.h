#pragma once
#include "Camera.h"
#include "Model.h"
#include "Structs.h"
#include "Texture.h"

#include <glew.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <string>
#include <sstream>
// take out if not req 
#include <SDL.h>

namespace GE {
	struct ShaderSource {
		std::string vertexSource;
		std::string fragmentSource;
	};

	static ShaderSource ParseShader(const std::string& filepath)
	{
		enum class ShaderType
		{
			NONE = -1,
			VERTEX = 0,
			FRAGMENT = 1

		};
		ShaderType type = ShaderType::NONE;

		// open file
		std::ifstream stream(filepath);
		std::stringstream ss[2];
		

		std::string line;
		while (std::getline(stream, line)) {
			// if line contains shader and not invalid set type to vertex
	
		
			if (line.find("#shader") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos) {
					type = ShaderType::VERTEX;
				}
				else if (line.find("fragment") != std::string::npos) {
					type = ShaderType::FRAGMENT;
				}
			}
			else {
				// based on type - write lines that do not start with vertex or fragment;
				ss[(int)type] << line << "\n";
			}
		}

		// return string from the string stream
		return { ss[0].str(), ss[1].str() };
	}

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

	class Renderer
	{
	public:
		Renderer();
		virtual ~Renderer();

		void init();

		void update();

		void draw(Camera* cam);

		void destroy();

		void addObject();

	private:
		GLuint programID;
		GLint vertexPosLocation;
		GLuint vboTraiangle;
		GLint vertexUVLocation;

		float pos_x, pos_y, pos_z;
		float rot_x, rot_y, rot_z;
		float scale_x, scale_y, scale_z;

		GLuint transformUniformId;
		GLuint viewUniformId;
		GLuint projectionUniformId;
		GLuint sampleId;

		Model* model;
		
		std::unique_ptr<Texture> texture;
		
		// not yet 
		std::vector <Model*> objects;


	};
}