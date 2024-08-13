#pragma once

#include<vector>
#include <GL/glew.h>

struct VertexBufferElement
{
	unsigned int count;
	unsigned int type;
	bool normalized;
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout()
		:m_Stride(0){}
	~VertexBufferLayout();

	template<typename T>
	void Push(int count) 
	{
		static_assert(false);
	}

	template<typename T>
	void Push <float>(int count)
	{
		m_Elements.push_back({ count, GL_FLOAT, GL_FALSE });
		m_Stride += sizeof(GLfloat);
	}

	template<typename T>
	void Push <unsigned int>(int count)
	{
		m_Elements.push_back({ count, GL_UNSIGNED_INT, GL_FALSE });
		m_Stride += sizeof(GLuint);
	}

	template<typename T>
	void Push <unsigned char>(int count)
	{
		m_Elements.push_back({ count, GL_UNSIGNED_BYTE, GL_TRUE });
		m_Stride += sizeof(GLbyte);
	}

	inline unsigned int GetStride() const { return m_Stride;  }

	inline const std::vector<VertexBufferElement> GetElement() const { return m_Elements;  }
};
