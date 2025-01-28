#pragma once

class IndexBuffer
{
private:
	unsigned int renderID;
	unsigned int count;

public:
	IndexBuffer(const unsigned int *, unsigned int);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return count; }
};