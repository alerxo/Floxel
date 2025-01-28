#include "Rendering/VertexArray.h"

#include "Rendering/Renderer.h"

VertexArray::VertexArray()
{
	GL_CALL(glGenVertexArrays(1, &rendererID));
	GL_CALL(glBindVertexArray(rendererID));
}

VertexArray::~VertexArray()
{
	GL_CALL(glDeleteVertexArrays(1, &rendererID));
}

void VertexArray::AddBuffer(const VertexBuffer &buffer, const VertexBufferLayout &layout)
{
	Bind();
	buffer.Bind();

	/* 
	Enabling and specifiyng the buffer layout of buffer using all elements
	Keeps track of offset with element size
	Attributes will be bound to vertex array object
	*/
	const std::vector<VertexBufferElement>& elements = layout.GetElements();
	intptr_t offset = 0;

	for(unsigned int i = 0; i < elements.size(); i++)
	{
		const VertexBufferElement element = elements[i];
		GL_CALL(glEnableVertexAttribArray(i));
		GL_CALL(glVertexAttribPointer(0, element.count, element.type, element.isNormalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GL_CALL(glBindVertexArray(rendererID));
}

void VertexArray::Unbind() const
{
#ifdef DEBUG
	GL_CALL(glBindVertexArray(0));
#endif // DEBUG
}