#include "Rendering/VertexBufferLayout.h"

void VertexBufferLayout::Push(unsigned int type, unsigned int count, unsigned char isNormalized)
{
	elements.push_back({type, count, isNormalized});
	stride += VertexBufferElement::GetSizeOfType(type) * count;
}