#include "IndexBuffer.h"

GE::IndexBuffer::IndexBuffer(const void* data, GLuint count) : count(count)
{
	GLCALL(glGenBuffers(1, &renderer_ID));
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_ID));
	GLCALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW));
}

GE::IndexBuffer::~IndexBuffer()
{
	GLCALL(glDeleteBuffers(1, &renderer_ID));
}

void GE::IndexBuffer::bind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer_ID));

}

void GE::IndexBuffer::unbind() const
{
	GLCALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

}
