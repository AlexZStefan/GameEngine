#pragma once
#include "Texture.h"
#include "Shader.h"

namespace GE {
	class Material {
	public:
		Material(Shared<Shader> _shader)
		{
			shader = _shader;
		};
		Material() {
		};
		~Material() {
		}

		Shared<Shader> shader;
	private:

	};

}
