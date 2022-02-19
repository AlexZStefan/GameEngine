#pragma once
#include  "Functions.h"
#include <glew.h>
namespace GE {

	class VertexBuffer
	{
	public:
		VertexBuffer(const void* data, GLuint size);
		~VertexBuffer();

		void bind() const;
		void unbind()const;

	private:
		GLuint renderer_ID;
	};


}
