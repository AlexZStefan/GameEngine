#pragma once
#include  "Functions.h"
#include <glew.h>
namespace GE {

	class IndexBuffer
	{
	public:
		IndexBuffer(const void* data, GLuint count);
		~IndexBuffer();

		void bind() const;
		void unbind() const;

		inline unsigned int GetCount() { return count; }
	private:
		GLuint renderer_ID;
		GLuint count;


	};


}
