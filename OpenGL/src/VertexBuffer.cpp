#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
    /* Generate buffer that openGL will draw from and assigns ID to m_RenderID. */
    GLCall(glGenBuffers(1, &m_RenderID));

    /* Bind Buffer */
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID));

    /* Initializes VertexBuffer's object data store. */
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
