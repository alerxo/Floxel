#pragma once

#include "VertexBufferElement.h"
#include <vector>

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> elements;
	unsigned int stride;

public:
	VertexBufferLayout() : stride(0) { }

	void Push(unsigned int, unsigned int, unsigned char);

	inline const std::vector<VertexBufferElement>& GetElements() const { return elements; }
	inline unsigned int GetStride() const { return stride; }
};