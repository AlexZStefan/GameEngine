#pragma once
#include "Texture.h"
#include "Structs.h"
#include "Functions.h"

namespace GE {

	class Billboard : public Object
	{
	public:
		Billboard(Shared<Texture> _t) {
			m_texture = _t;
		}

		~Billboard() {
			
			// gets deleted in Texture.h
			//delete m_texture;
		}

		Shared<Texture> getTexture() {
			return m_texture;
		}

	private:
		Shared<Texture> m_texture;
		


	};

}
