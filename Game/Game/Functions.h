#pragma once
#include <glew.h>
#include <SDL.h>
#include <glm/glm.hpp>
#include <vector>

namespace GE {

	class DrawLine
	{
	public:

		DrawLine();
		
		~DrawLine();

		void draw(glm::vec3 start, glm::vec3 end);
		
	private:

		int shaderProgram;
		unsigned int VBO, VAO;
		std::vector<float> vertices;
		glm::vec3 startPoint;
		glm::vec3 endPoint;
		glm::mat4 MVP = glm::mat4(1.0f);
		glm::vec3 lineColour;

	};

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