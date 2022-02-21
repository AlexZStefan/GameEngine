#pragma once

#include "Structs.h"
#include "Texture.h"

#include <glew.h>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <glew.h>

#define ASSERT(xer) if(!(xer)) __debugbreak();
#define GLCALL(xer) GLClearError();\
	xer;\
	ASSERT(GLLogCall())

// clear errors
static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

// show all errors
static bool GLLogCall() {
	while (GLenum err = glGetError()) {
		std::cerr << "OpenGl Error : " << err << std::endl;
		return false;
	}
	return true;
}

namespace GE {

	class Model
	{
	public:
		Model();

		~Model();

		bool loadFromFile(const char* filename, bool flipUV);

		GLuint getVerts() {
			return vbo;
		}

		unsigned int getNumVertices() {
			return numVertices;
		}

		GLuint getMaterial() {
			return material;
		}

		void setMaterial(GLuint mat) {
			material = mat;
		}

		virtual void setAttribute(GLint position, int posSize, GLint uv, int uvSize);

		 virtual void bindVBO();
		 virtual void unbindVBO();

		 virtual void bindIBO();
		 virtual void unbindIBO();

		 virtual void bindTexture(GLuint sample);
		 virtual void unbindTexture();

	protected:
		GLuint vbo;
		GLuint ibo;
		unsigned int numVertices;
		GLuint material;

		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		//std::unique_ptr<Texture> texture; 
	};

}
