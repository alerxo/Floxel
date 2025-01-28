#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
	unsigned int rendererID;

public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer &, const VertexBufferLayout &);
	void Bind() const;
	void Unbind() const;
};