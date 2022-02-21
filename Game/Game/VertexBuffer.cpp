#include "VertexBuffer.h"

GE::VertexBuffer::VertexBuffer(GLuint size, const void* data)
{
	GLCALL(glGenBuffers(1, &renderer_ID));
	GLCALL(glBindBuffer( GL_ARRAY_BUFFER, renderer_ID));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
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
