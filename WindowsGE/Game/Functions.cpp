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

			std::string error;
			switch (err)
			{
			case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
			}
			std::cerr << "OpenGl Error : " << error <<" " << err << std::endl;
			return false;
		}
		return true;
	}

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

	/*DrawLine::DrawLine()
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


	}*/

	//DrawLine::~DrawLine()
	//{
	//}

	//void DrawLine::draw(glm::vec3 start, glm::vec3 end)
	//{
	//	float x1 = start.x * 2 / (float)980;
	//	float y1 = start.y * 2 / (float)980;
	//	float z1 = start.z * 2 / (float)980;
	//}
}