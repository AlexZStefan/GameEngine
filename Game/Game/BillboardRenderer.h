#pragma once
#include "Billboard.h"

#include <glm/glm.hpp>
#include <iostream>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Billboard;

namespace GE {
	class BillboardRenderer : public Renderer
	{
	public:
		BillboardRenderer(Billboard* billboard);

		~BillboardRenderer() {
		}

		void init();

		void draw(Billboard* billboard, Camera *cam);
	private:
	};
}
