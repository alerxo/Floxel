#pragma once

class VertexBuffer
{
private:
	unsigned int renderID;

public:
	VertexBuffer(const void *, unsigned int);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};