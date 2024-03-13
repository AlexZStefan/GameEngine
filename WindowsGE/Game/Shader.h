 #pragma once
#include "Functions.h"

#include <string>
#include <GL/glew.h>
#include <iostream>
#include <iostream>
#include <fstream>
#include <unordered_map>

namespace GE {

	class Shader
	{
	public:
		Shader();
		~Shader();

		void bind();
		void unbind();
		virtual int loadShader(std::string vert_filepath, std::string frag_filepath );
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniform3f(const std::string& name, float v0, float v1, float v2);
		void SetUniform2f(const std::string& name, float v0, float v1);
		void SetUniform3i(const std::string& name, int v0, int v1, int v2);
		void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
		unsigned int getProgramId() { return m_ProgramID; };

	protected:
		unsigned int m_ProgramID;
		GLint GetUniformLocation(const std::string& name);
		int CompileShader(const GLchar* v_shader_sourcecode[], const GLchar* f_shader_sourcecode[]);
		std::unordered_map<std::string, int> m_unfiromLocationCache;
	};


}