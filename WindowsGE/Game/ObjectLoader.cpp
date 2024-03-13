//#include "ObjectLoader.h"
//
//bool GE::ObjectLoader::loadFromFile(const char* filename, bool flipUV)
//{
//	// create object importer
//	Assimp::Importer imp;
//	const aiScene* pScene = nullptr;
//
//	if (flipUV)
//		pScene = imp.ReadFile(filename, aiProcessPreset_TargetRealtime_Quality | aiProcess_FlipUVs);
//	else if (!flipUV)
//	{
//		pScene = imp.ReadFile(filename, aiProcessPreset_TargetRealtime_Quality);
//	}
//
//	if (!pScene)
//		return false;
//	// for every obj in scene set curent mesh
//	for (int MeshIdx = 0; MeshIdx < pScene->mNumMeshes; MeshIdx++) {
//		const aiMesh* mesh = pScene->mMeshes[MeshIdx];
//		// get face of of the object
//		for (int faceIdx = 0; faceIdx < mesh->mNumFaces; faceIdx++) {
//			const aiFace& face = mesh->mFaces[faceIdx];
//			// get vertecies of face index 
//			for (int vertIdx = 0; vertIdx < 3; vertIdx++) {
//				// set curent vector to the coresponding vertex of the face idx 
//				const aiVector3D* pos = &mesh->mVertices[face.mIndices[vertIdx]];
//
//				const aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[vertIdx]];
//
//				vertices.push_back(Vertex(pos->x, pos->y, pos->z, uv.x, uv.y));
//			}
//		}
//	}
//
//	numVertices = vertices.size();
//	indexCount = indices.size();
//
//	GLCALL(glGenBuffers(1, &vbo));
//	GLCALL(glGenBuffers(1, &ibo));
//	return true;
//}