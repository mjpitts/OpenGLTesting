#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    /* Generate buffer that openGL will draw from and assigns ID to the unsigned int address. */
    GLCall(glGenBuffers(1, &m_RenderID));
    /* Set use of Buffer and bind buffer to ID. */
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID));
    /* Creates and initializes a buffer object's data store. */
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RenderID));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
