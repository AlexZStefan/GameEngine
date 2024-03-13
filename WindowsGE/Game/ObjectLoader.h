#pragma once
#include "Model.h"

namespace GE {

	class ObjectLoader : public Model
	{
	public:
		ObjectLoader();
		~ObjectLoader();

		bool loadFromFile(const char* filename, bool flipUV);
	private:
	};

}
