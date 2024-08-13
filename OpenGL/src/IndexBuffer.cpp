#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
    :m_Count(count)
{

    /* Possibility these won't be equal which could cause buffer sizing issues. */
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    /* Generate buffer that openGL will draw from and assigns ID to the unsigned int address. */
    GLCall(glGenBuffers(1, &m_RenderID));
    /* Set use of Buffer and bind buffer to ID. */
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID));
    /* Creates and initializes a buffer object's data store. */
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RenderID));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
