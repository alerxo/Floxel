#include "Rendering/VertexBuffer.h"

#include "Rendering/Renderer.h"
#include <glad/glad.h>

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	/* Creating buffer and filling it with vertex position data */
	GL_CALL(glGenBuffers(1, &renderID));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, renderID));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GL_CALL(glDeleteBuffers(1, &renderID));
}

void VertexBuffer::Bind() const
{
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, renderID));
}

void VertexBuffer::Unbind() const
{
#ifdef DEBUG
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
#endif // DEBUG
}