#pragma once

#include <GL/glew.h>

#include<vector>
#include <stdexcept>

#include "Renderer.h"



struct VertexBufferElement
{
	unsigned int count;
	unsigned int type;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch(type)
		{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
		}
		
		/* Assert false means that this assert is always triggered
		if the code executes to here. */
		ASSERT(false);
		return 0;
	}
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
	void Push(unsigned int count) 
	{
		std::runtime_error(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ count, GL_FLOAT, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ count, GL_UNSIGNED_INT, GL_FALSE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ count, GL_UNSIGNED_BYTE, GL_TRUE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline unsigned int GetStride() const { return m_Stride;  }

	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements;  }
};
