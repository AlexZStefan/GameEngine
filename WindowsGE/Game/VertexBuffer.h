#pragma once
#include  "Functions.h"
#include <glew.h>
namespace GE {

	class VertexBuffer
	{
	public:
		VertexBuffer(unsigned int size, const void* data, int drawType = 0);
		~VertexBuffer();
			
		void bind() const;
		void unbind()const;

		GLuint renderer_ID;
	private:
	};


}
