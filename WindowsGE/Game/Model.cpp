#include "Model.h"

namespace GE {

	GE::Model::Model(Shared<Shader> _shader)
	{
		numVertices = 0;
		indexCount = 0;
		mat =  std::make_shared<Material>();
		m_ShaderID = _shader->getProgramId();
	}

	GE::Model::~Model()
	{
	}
	GE::Model::Model()
	{
		numVertices = 0;
		indexCount = 0;
	}

	void Model::SetModel(Shared<Model> m)
	{
		vertices = m->vertices;
		indexCount = m->indexCount;
		indices = m->indices;
		ib = m->ib;
		name = m->name;
		m_ShaderID = m->m_ShaderID;
		material = m->material;
		mat = m->mat;
		normalID = m->normalID;
		m_hasNormals = m->m_hasNormals;
		numVertices = m->numVertices;
		Init();
		//default shader
	}

	void Model::Init()
	{		
		vb = std::make_shared<VertexBuffer>(getNumVertices() * sizeof(Vertex), vertices.data());
		vl = std::make_shared<VertexBufferLayout>();
		vl->push<float>(3);
		vl->push<float>(2);
		if (hasNormals()) {
			vl->push<float>(3);
		}
		ib = std::make_shared<IndexBuffer>(indices.size() , indices.data());
		ibo = ib->renderer_ID;
		ib->bind();

		va = std::make_shared<VertexArray>();
		va->addBuffer(vb, vl, 0);
		va->unbind();
		vb->unbind();
		ib->unbind();
		/*vb = std::make_shared<VertexBuffer>
			(getNumVertices() * sizeof(Vertex), vertices.data());
		vl = std::make_shared<VertexBufferLayout>();
		vl->push<float>(3);
		vl->push<float>(2);
		if (hasNormals()) {
			vl->push<float>(3);
		}

		va = std::make_shared<VertexArray>();
		va->addBuffer(vb, vl, 0);
		va->unbind();
		vb->unbind();*/		
	}
	bool GE::Model::loadFromFile(const char* filename, bool flipUV)
	{
		// create object importer
		Assimp::Importer imp;
		const aiScene* pScene = nullptr;

		if (flipUV)
			// additional flags | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices |aiProcess_GenSmoothNormals
			pScene = imp.ReadFile(filename, aiProcessPreset_TargetRealtime_Quality | aiProcess_FlipUVs);
		else if (!flipUV)
		{
			pScene = imp.ReadFile(filename, aiProcessPreset_TargetRealtime_Quality);
		}
		if (!pScene)
			return false;
		if (pScene->mNumMeshes > 1)
		{
			std::cout << "AI MESH COMPONENTS SIZE : " << pScene->mNumMeshes <<
				"and has " << pScene->mNumMaterials << "materials" <<
				"file name is " << filename << std::endl;
		}

		// for every obj component loop through the array
		for (int MeshIdx = 0; MeshIdx < pScene->mNumMeshes; MeshIdx++) {
			// get the first component of the mesh
			const aiMesh* mesh = pScene->mMeshes[MeshIdx];

			indexCount = mesh->mNumFaces * 3;
			// loop through faces of the component
			for (int faceIdx = 0; faceIdx < mesh->mNumFaces; faceIdx++) {
				const aiFace& face = mesh->mFaces[faceIdx];
				assert(face.mNumIndices == 3);

				for (int i = 0; i < face.mNumIndices; i++) {
					indices.push_back(face.mIndices[i]);
				}

				// get vertecies of face 
				for (int vertIdx = 0; vertIdx < 3; vertIdx++) {
					// get position, texture and normals of the component 
					const aiVector3D* pos = &mesh->mVertices[face.mIndices[vertIdx]];
					const aiVector3D* uv = &mesh->mTextureCoords[0][face.mIndices[vertIdx]];
					if (mesh->HasNormals())
					{
						m_hasNormals = 1;
						const aiVector3D* n = &mesh->mNormals[face.mIndices[vertIdx]];
						vertices.push_back(Vertex(pos->x, pos->y, pos->z, uv->x, uv->y, n->x, n->y, n->z));
					}
					else {
						m_hasNormals = 0;
						vertices.push_back(Vertex(pos->x, pos->y, pos->z, uv->x, uv->y, 0, 0, 0));
					}}}
		}
		numVertices = vertices.size();
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
		return true;
	}
	void Model::unbindTexture()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void Model::BindVA()
	{
		va->bind();
	}
	void Model::UnbindVA()
	{
		va->unbind();
	}
	 void Model::bindIBO()
	 {
		 if (indices.size() > 0)
		 {
			 ib->bind();
;		 //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		 //glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
		 }
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
	 void Model::bindNormalMap()
	 {
		 glActiveTexture(GL_TEXTURE2);
		 glUniform1i(normalID, 2);
		 glBindTexture(GL_TEXTURE_2D, normalID);
	 }
}
