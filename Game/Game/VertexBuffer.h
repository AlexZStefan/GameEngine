#pragma once
#include  "Functions.h"
#include <glew.h>
namespace GE {

	class VertexBuffer
	{
	public:
		VertexBuffer(GLuint size, const void* data);
		~VertexBuffer();

		void bind() const;
		void unbind()const;

	private:
		GLuint renderer_ID;
	};


}
