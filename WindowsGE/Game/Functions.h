#pragma once

#include <glew.h>
#include <SDL.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>

#define ASSERT(xer) if(!(xer)) __debugbreak();
#define GLCALL(xer) GLClearError();\
	xer;\
	ASSERT(GLLogCall())


namespace GE {

	void GLClearError();
	bool GLLogCall();
	void displayShaderCompilerError(GLuint shaderId);

	template <typename T>
	using Shared = std::shared_ptr<T>;


	//class DrawLine
	//{
	//public:

	//	DrawLine();
	//	
	//	~DrawLine();

	//	void draw(glm::vec3 start, glm::vec3 end);
	//	
	//private:

	//	int shaderProgram;
	//	unsigned int VBO, VAO;
	//	std::vector<float> vertices;
	//	glm::vec3 startPoint;
	//	glm::vec3 endPoint;
	//	glm::mat4 MVP = glm::mat4(1.0f);
	//	glm::vec3 lineColour;

	//};

	/*
	SINGLETON PATT

	class DrawLine
	{
	public:

		DrawLine(const DrawLine&) = delete;
		~DrawLine();

		void draw(glm::vec3 from, glm::vec3 to);

		static DrawLine& Get() {
			return s_Instance;
		};

	private:
		DrawLine(){};

		static DrawLine s_Instance;

		int shaderProgram;
		unsigned int VBO, VAO;
		std::vector<float> vertices;
		glm::vec3 startPoint;
		glm::vec3 endPoint;
		glm::mat4 MVP = glm::mat4(1.0f);
		glm::vec3 lineColour;

	};
	*/

	

}