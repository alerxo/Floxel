#pragma once

#include <vector>
#include <glad/glad.h>

namespace Renderer
{
	void Initialize();
	void Destroy();

	void RenderFrame();
};

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

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char isNormalized;

	static unsigned int GetSizeOfType(unsigned int type);
};

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