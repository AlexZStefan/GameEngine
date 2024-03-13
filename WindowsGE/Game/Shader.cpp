#include "Shader.h"

namespace GE {
	Shader::Shader() {
		m_ProgramID = 0;

		// load a basic shader
		bind();
	}

	int Shader::loadShader(std::string vert_filepath , std::string frag_filepath ) {

		// load the vertex shader 
		std::ifstream vifs(vert_filepath);
		if (!vifs.is_open()) {
			std::cerr << "Problem opening file: " << vert_filepath << " Check file is in the directory" << std::endl;
			return -1;
		}

		std::string v_shader_source((std::istreambuf_iterator<char>(vifs)),
			(std::istreambuf_iterator<char>()));
		
		const GLchar* v_source_array[] = { v_shader_source.c_str() };

		vifs.clear();
		vifs.close();

		// load the fragment shader
		vifs.open(frag_filepath);
		if (!vifs.is_open()) {
			std::cerr << "Problem opening file: " << vert_filepath << " Check file is in the directory" << std::endl;
			return -1;
		}

		std::string f_shader_source((std::istreambuf_iterator<char>(vifs)),
			(std::istreambuf_iterator<char>()));
		const GLchar* f_source_array[] = { f_shader_source.c_str() };

		vifs.clear();
		vifs.close();

		int result = CompileShader(v_source_array, f_source_array);
		if (result != 0) {
			std::cerr << "Failed to compile program " << std::endl;
			return -1;
		}
		return result;
	}

	Shader::~Shader()
	{
		GLCALL(glDeleteProgram(m_ProgramID));
	}

	void Shader::bind()
	{
		GLCALL(glUseProgram(m_ProgramID));
	}

	void Shader::unbind()
	{
		GLCALL(glUseProgram(0));
	}

	void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
	{
		GLCALL(glProgramUniform4f(m_ProgramID, GetUniformLocation(name), v0, v1, v2, v3));
	}

	void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
	{
		GLCALL(glProgramUniform3f(m_ProgramID, GetUniformLocation(name), v0, v1, v2));
	}

	void Shader::SetUniform2f(const std::string& name, float v0, float v1)
	{
		GLCALL(glProgramUniform2f(m_ProgramID, GetUniformLocation(name), v0, v1));
	}

	void Shader::SetUniform3i(const std::string& name, int v0, int v1, int v2)
	{
		GLCALL(glProgramUniform3i(m_ProgramID, GetUniformLocation(name), v0, v1, v2));
	}	

	void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
		GLCALL(glProgramUniformMatrix4fv(m_ProgramID,
			GetUniformLocation(name),1, GL_FALSE, &matrix[0][0]));
	}
	
	GLint Shader::GetUniformLocation(const std::string& name)
	{
		// search through and if contains the name -return else cache it
		if (m_unfiromLocationCache.find(name) != m_unfiromLocationCache.end())
			return m_unfiromLocationCache[name];

		GLCALL(GLint location = glGetUniformLocation(m_ProgramID, name.c_str()));
		if (location == -1)  
			std::cout << "Uniform "<< name << " does not exist!" << std::endl;
		
		m_unfiromLocationCache[name] = location;
		return location;
	}

	int Shader::CompileShader(const GLchar* v_shader_sourcecode[], const GLchar* f_shader_sourcecode[])
	{
		// create and compile vertex and fragment shader
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, v_shader_sourcecode, nullptr);
		glCompileShader(vertexShader);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, f_shader_sourcecode, NULL);
		glCompileShader(fragmentShader);
		
		m_ProgramID = glCreateProgram();
		
		GLint isShaderCompiledOK = GL_FALSE;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isShaderCompiledOK);
		if (isShaderCompiledOK != GL_TRUE) {
			std::cerr << "Unable to compile vertex shader" << std::endl;
			displayShaderCompilerError(vertexShader);
			return -1;
		}
		isShaderCompiledOK = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isShaderCompiledOK);
		if (isShaderCompiledOK != GL_TRUE) {
			std::cerr << "Unable to compile fragment shader" << std::endl;
			displayShaderCompilerError(fragmentShader);
			return -1;
		}

		// attach shader to program and link to create executable program
		// used to render objects - executable exists in graphics memory
		glAttachShader(m_ProgramID, vertexShader);
		glAttachShader(m_ProgramID, fragmentShader);
		glLinkProgram(m_ProgramID);
		glValidateProgram(m_ProgramID);

		// Check for linking errors
		GLint isProgramLinked = GL_FALSE;
		glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &isProgramLinked);
		if (isProgramLinked != GL_TRUE) {
			std::cerr << "Failed to link program" << std::endl;
			return -1;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return 0;
	}
}