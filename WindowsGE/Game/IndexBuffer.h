#pragma once
#include  "Functions.h"
#include <glew.h>
#include "Structs.h"
namespace GE {

	class IndexBuffer
	{
	public:
		IndexBuffer(unsigned int count, const void* data);
		~IndexBuffer();

		void bind() const;
		void unbind() const;

		inline unsigned int getCount() const { return count; };

		inline unsigned int GetCount() { return count; }
		GLuint renderer_ID;
	private:
		unsigned int count;

	};


}
