#include "Rendering/VertexBufferElement.h"

#include "Rendering/Renderer.h"

unsigned int VertexBufferElement::GetSizeOfType(unsigned int type)
{
	switch (type)
	{
	case GL_FLOAT:
		return 4;

	case GL_UNSIGNED_INT:
		return 4;

	case GL_UNSIGNED_BYTE:
		return 1;

	default:
		LOG_ERROR("Defaulted");
		return 0;
	}
}