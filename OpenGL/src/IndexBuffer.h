#pragma once

class IndexBuffer
{
private:
	/* Id for openGl sate machine. */
	unsigned int m_RenderID;
	unsigned int m_Count;
public:
	/* count means element count. */
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};