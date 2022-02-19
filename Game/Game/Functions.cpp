#include "Functions.h"


namespace GE
{



	// clear errors
	 void GLClearError() {
		while (glGetError() != GL_NO_ERROR);
	}
	// show all errors
	bool GLLogCall() {
		while (GLenum err = glGetError()) {
			std::cerr << "OpenGl Error : " << err << std::endl;
			return false;
		}
		return true;
	}


	/*Vector3 startPos = Vector3(12, 23, 42);
	Vector3 endPos = Vector3(2, 2, 2);*/
	
	/*glm::vec3 firsts(5, 10, 20);
	glm::vec3 _2nd(5, 10, 20);
	glm::vec3 result = _2nd * firsts;*/

	inline std::ostream& operator<< (std::ostream& stream, const glm::vec3& other) {

		stream << other.x << "," << other.y << "," << other.z;
		return stream;
	}

	inline const glm::vec3& operator*(glm::vec3& thisOne, const glm::vec3& other) {

		return glm::vec3(thisOne.x * other.x, thisOne.y * other.y, thisOne.z * other.z);
	}

	DrawLine::DrawLine()
	{
		static const char* vertexShaderSource =
			"#version 330 core\n"
			"in vec3 aPos;\n"
			"uniform mat4 MVP;\n"
			"void main()\n"
			"{\n"
			"gl_Position = MVP * vec4(aPos.x, aPos.y, aPos.z, 1.0f;\n"
			"}\0";

		static const char* fragmentShaderSource = 
			"#version 330 core\n"
			"out vec4 FragCol;\n"
			"uniform vec3 color;\n"
			"void main()\n"
			"{\n"
			"FragCol = vec4(color, 1.0f);\n"
			"}\0";

		int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 3, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 4, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		shaderProgram = glCreateProgram();


	}

	DrawLine::~DrawLine()
	{
	}

	void DrawLine::draw(glm::vec3 start, glm::vec3 end)
	{
		float x1 = start.x * 2 / (float)980;
		float y1 = start.y * 2 / (float)980;
		float z1 = start.z * 2 / (float)980;
	}
}