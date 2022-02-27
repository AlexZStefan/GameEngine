#include "Model.h"

namespace GE {

	GE::Model::Model()
	{
		vbo = 0;
		numVertices = 0;
		indexCount = 0;

		GLCALL(glGenBuffers(1, &vbo));
		GLCALL(glGenBuffers(1, &ibo));
	}

	GE::Model::~Model()
	{
		GLCALL(glDeleteBuffers(1, &vbo));
		GLCALL(glDeleteBuffers(1, &ibo));
	}

	bool GE::Model::loadFromFile(const char* filename, bool flipUV)
	{
		// create object importer
		Assimp::Importer imp;
		const aiScene* pScene = nullptr;

		if (flipUV)
			pScene = imp.ReadFile(filename, aiProcessPreset_TargetRealtime_Quality | aiProcess_FlipUVs);
		else if (!flipUV)
		{
			pScene = imp.ReadFile(filename, aiProcessPreset_TargetRealtime_Quality);
		}

		if (!pScene)
			return false;
		// for every obj in scene set curent mesh
		for (int MeshIdx = 0; MeshIdx < pScene->mNumMeshes; MeshIdx++) {
			const aiMesh* mesh = pScene->mMeshes[MeshIdx];
			// get face of of the object
			for (int faceIdx = 0; faceIdx < mesh->mNumFaces; faceIdx++) {
				const aiFace& face = mesh->mFaces[faceIdx];
				// get vertecies of face index 
				for (int vertIdx = 0; vertIdx < 3; vertIdx++) {
					// set curent vector to the coresponding vertex of the face idx 
					const aiVector3D* pos = &mesh->mVertices[face.mIndices[vertIdx]];

					const aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[vertIdx]];

					vertices.push_back(Vertex(pos->x, pos->y, pos->z, uv.x, uv.y));
				}
			}
		}

		numVertices = vertices.size();
		indexCount = indices.size();

		return true;
	}

	bool Model::loadModel(std::vector<Vertex> _model, std::vector<int> _indices)
	{
		std::vector<Vertex>::iterator it;
		std::vector<int>::iterator iit = _indices.begin();
		for (it = _model.begin(); it < _model.end(); it++)
		{

			vertices.push_back(*it); 

			
			if (iit != _indices.end())
			{
				indices.push_back(*iit); 
				std::advance(iit, +1);
			}
		}
		numVertices = vertices.size();

		GLCALL(glGenBuffers(1, &vbo));
		GLCALL(glGenBuffers(1, &ibo));
		return true;
	}

	void Model::setAttribute(GLint position, int posSize, GLint uvLocation, int uvSize)
	{
		glEnableVertexAttribArray(position);

		GLCALL(glVertexAttribPointer(position, posSize,
			GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x)));

		GLCALL(glVertexAttribPointer(uvLocation, uvSize,
			GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u)));
	}

	void Model::bindVBO()
	{
		 GLCALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));

		 GLCALL(glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW));
	}

	 void Model::unbindVBO()
	 {
		 glBindBuffer(GL_ARRAY_BUFFER, 0);
	 }

	 void Model::bindIBO()
	 {
		 glEnableVertexAttribArray(0);
		 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		 glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
	 }

	 void Model::unbindIBO()
	 {
		 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	 }

	 void Model::bindTexture(GLuint sample)
	 {
		 glActiveTexture(GL_TEXTURE0);
		 glUniform1i(sample, 0);
		 glBindTexture(GL_TEXTURE_2D, material);
	 }

	 void Model::unbindTexture()
	 {
		 glBindTexture(GL_TEXTURE_2D, 0);
	 }
}
