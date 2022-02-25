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
		bool loadModel(std::vector<Vertex> _model, std::vector<int> _indices);

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

		GLuint &getIndices() { return ibo; };
		unsigned int getIndicesCount() { return indexCount; };

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
		unsigned int indexCount;
		GLuint material;

		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		//std::unique_ptr<Texture> texture; 
	};
}
