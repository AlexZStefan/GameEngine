#include "Renderer.h"

namespace GE {

	Vertex triangleVertecies[3] = {
		Vertex(-1.0f, .0f, .0f),
		Vertex(1.0f, .0f, .0f),
		Vertex(.0f, 1.0f, .0f),

	};

	GLfloat vertexData[] = {
		-1.f, .0f,
		1.f, .0f,
		.0f, 1.f,
	};

	void displayShaderCompilerError(GLuint shaderId) {
		GLint MsgLen = 0;

		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &MsgLen);

		if (MsgLen > 1) {
			//GLchar* Msg = new GLchar[MsgLen + 1];

			GLchar* Msg = (GLchar*)alloca(MsgLen * sizeof(GLchar));

			glGetShaderInfoLog(shaderId, MsgLen, NULL, Msg);

			std::cerr << "Error compiling shader\n" << Msg << std::endl;

			//delete[] Msg;
			glDeleteShader(shaderId);
			
		}
	}


	Renderer::Renderer()
	{

	}


	Renderer::~Renderer()
	{
	}

	void Renderer::init()
	{

		// create vertex shader
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		const GLchar* V_ShaderCode[] = {
			"#version 140\n"
			"in vec2 vertexPos2D;\n"
			"void main()\n"
			"{\n"
			"gl_Position = vec4(vertexPos2D.x, vertexPos2D.y, 0,1);\n"
			"}\n"
		};

		// Copy the source to gl, ready for compilation
		glShaderSource(vertexShader, 1, V_ShaderCode, NULL);
		// compile the code
		glCompileShader(vertexShader);
		// check for errors - presume shader did not compile
		GLint isFShaderCompiledOK = GL_FALSE;
		// get  compilation status from openGL
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isFShaderCompiledOK);

		if (isFShaderCompiledOK != GL_TRUE) {
			std::cerr << "Unable to compile vertex shader" << std::endl;
			displayShaderCompilerError(vertexShader);
			return;
		}
	
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		const GLchar* F_ShaderCode[] = {
			"#version 140\n"
			"out vec4 fragmentColour;\n"
			"void main()\n"
			"{\n"
			"fragmentColour = vec4(1.0, 0.0, 0.0, 1.0);\n"
			"}\n"
		};

		glShaderSource(fragmentShader, 1, F_ShaderCode, NULL);
		glCompileShader(fragmentShader);
		 
		GLint isShaderCompiledOK = GL_FALSE;

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isShaderCompiledOK);

		if (isShaderCompiledOK != GL_TRUE) {
			std::cerr << "Unable to compile the fragment shader" << std::endl;

			displayShaderCompilerError(fragmentShader);
			return;
		}


		// create program and store it`s ID 
		programID = glCreateProgram();

		// attach shader to program obj 
		glAttachShader(programID, vertexShader);
		glAttachShader(programID, fragmentShader);

		// link program to create executable program and use to render objects
		// this will exists in graphics memory 
		glLinkProgram(programID);

		GLint isProgramLinked = GL_FALSE;

		// shader error check 
		glGetProgramiv(programID, GL_LINK_STATUS, &isProgramLinked);
		if (isProgramLinked != GL_TRUE) {
			std::cerr << "Failed to link program" << std::endl;
		}

		vertexPos2DLocation = glGetAttribLocation(programID, "vertexPos2D");

		if (vertexPos2DLocation == -1) {
			std::cerr << "Problem getting vertex2DPos" << std::endl;
		}

		// create vertex buffer object
		glGenBuffers(1, &vboTraiangle);
		glBindBuffer(GL_ARRAY_BUFFER, vboTraiangle);

		// transfer vertecies to graphic memory 
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	}

	void Renderer::update()
	{
	}

	void Renderer::draw()
	{
		glUseProgram(programID);

		glVertexAttribPointer(vertexPos2DLocation, 2, GL_FLOAT, GL_FLOAT, sizeof(GLfloat) * 2, nullptr);

		glEnableVertexAttribArray(vertexPos2DLocation);

		glBindBuffer(GL_ARRAY_BUFFER, vboTraiangle);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDisableVertexAttribArray(vertexPos2DLocation);

		glUseProgram(0);
	}

	void Renderer::destroy()
	{
		// Release object allocated for program and vertex buffer obj 
		glDeleteProgram(programID);
		glDeleteBuffers(1, &vboTraiangle);
	}

}