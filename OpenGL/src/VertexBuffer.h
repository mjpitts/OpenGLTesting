#pragma once

class VertexBuffer
{
private:
	/* Id for openGl sate machine. */
	unsigned int m_RenderID;
public:
	/* Size is in bytes. */
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;
};