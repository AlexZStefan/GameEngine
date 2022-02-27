#pragma once
#include "Renderer.h"

namespace GE {

	

	class Billboard : public Model
	{
	public:
		Billboard() : Model() {
			
		};
		~Billboard() {};




		void createBillboard() {
			Vertex billboardStruct[6] = {
			Vertex(0.5f, 1.0f, 0.0f, 1.0f, 1.0f),
			Vertex(-0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
			Vertex(-0.5f, 0.0f, 0.0f, 0.0f, 0.0f),

			Vertex(-0.5f, 0.0f, 0.0f, 0.0f, 0.0f),
			Vertex(0.5f, 0.0f, 0.0f, 1.0f, 0.0f),
			Vertex(0.5f, 1.0f, 0.0f, 1.0f, 1.0f)
			};
			
			for (auto e : billboardStruct) vertices.push_back(e);

			numVertices = vertices.size();
			indexCount = indices.size();
		}

	private:
	};

}
