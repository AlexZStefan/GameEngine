#include "VertexBuffer.h"

GE::VertexBuffer::VertexBuffer(const void* data, GLuint size)
{
	GLCALL(glGenBuffers(1, &renderer_ID));
	GLCALL(glBindBuffer( GL_ARRAY_BUFFER, renderer_ID));
	GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data,GL_STATIC_DRAW));
}

GE::VertexBuffer::~VertexBuffer() const
{
	GLCALL(glDeleteBuffers(1, &renderer_ID));
}

void GE::VertexBuffer::bind() const
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, renderer_ID));

}

void GE::VertexBuffer::unbind()
{
	GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

}
