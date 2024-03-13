#include "VertexBuffer.h"

// drawType 0 = static, 1 = strean, 2 = dynamic 
GE::VertexBuffer::VertexBuffer(unsigned int size, const void* data, int drawType)
{
	GLCALL(glGenBuffers(1, &renderer_ID));
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, renderer_ID));

	switch (drawType)
	{
	case 1: 
		GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW));
		break;
	case 2: 
		GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
		break;
	default:
		GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
		break;
	}
}

GE::VertexBuffer::~VertexBuffer() 
{
	GLCALL(glDeleteBuffers(1, &renderer_ID));
}

void GE::VertexBuffer::bind() const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, renderer_ID));
}

void GE::VertexBuffer::unbind()const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

