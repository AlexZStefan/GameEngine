#pragma once

#include "Structs.h"
#include "Texture.h"
#include "Functions.h"
#include "VertexArray.h"

#include <glew.h>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <glew.h>
#include <math.h>
#include <GL/glew.h>
#include "systems/ECS/ECS.h"
#include "Material.h"

#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals || aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices)

namespace GE {
	class Mesh {
	public:
		Mesh() {}
		~Mesh() {}
	private:
	};

	class Model : public Object
	{
	public:
		Model(Shared<Shader> _shader);
		~Model();
		Model();

		void SetModel(Shared<Model> m);

		void Init();

		bool loadFromFile(const char* filename, bool flipUV);
		void Init(Shared<Material> _shader);
		bool loadModel(std::vector<Vertex> _model, std::vector<int> _indices);
		unsigned int getNumVertices() {
			return numVertices;
		}
		GLuint getMaterial() {
			return material;
		}
		void setMaterial(GLuint mat) {
			material = mat;
		}
		GLuint getNormalMap() {
			return normalID;
		}
		void setNormal(GLuint nMap) {
			normalID = nMap;
		}
		Model& operator=(const Model other) {
			if (this != &other) {
				// Perform the assignment
				this->vb = other.vb;
				this->vertices = other.vertices;
				this->ibo = other.ibo;
				this->indexCount = other.indexCount;
				this->numVertices = other.numVertices;
				this->numVertices = other.numVertices;
				this->va = other.va;
			}
			// Return the current object by reference
			return *this;
		}
		unsigned int getIndicesCount() { return indexCount; };
		void bindIBO();
		void unbindIBO();
		virtual void bindTexture(GLuint sample);
		void bindNormalMap();
		virtual void unbindTexture();
		void calculateNormals();
		// used in standard shader initialization
		bool hasNormals() { return m_hasNormals; }
		// used with bump map shader
		void BindVA();
		void UnbindVA();
		void set_name(std::string _name) { name = _name; };
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<Vertex>& getVertices() { return vertices; };
		Shared<Material> mat;
		std::string name;

		unsigned int indexCount;
		unsigned int m_ShaderID;
		unsigned int numVertices;
		bool m_hasNormals;
		GLuint material;
		GLuint normalID;
		//std::unique_ptr<Texture> texture; 
		GLuint ibo;
		bool modelLoaded = false;
	protected:
	private:

		Shared<VertexBuffer> vb;
		Shared<IndexBuffer> ib;
		Shared<VertexArray> va;
		Shared<VertexBufferLayout> vl;
	};


}
