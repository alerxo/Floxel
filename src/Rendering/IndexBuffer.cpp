#include "Rendering/IndexBuffer.h"

#include "Rendering/Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count) : count(count)
{
	ASSERT(sizeof(unsigned int), sizeof(GLuint), "Mismatch of type size");

	GL_CALL(glGenBuffers(1, &renderID));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderID));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * count, data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GL_CALL(glDeleteBuffers(1, &renderID));
}

void IndexBuffer::Bind() const
{
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderID));
}

void IndexBuffer::Unbind() const
{
#ifdef DEBUG
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
#endif // DEBUG
}