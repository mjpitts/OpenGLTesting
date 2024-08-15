#pragma once

#include<GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError;\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

/* This function clears openGL error flags, but does not print them. */
void GLClearError();

/* This function clears the openGL error flags and prints them as it goes. */
bool GLLogCall(const char* function, const char* file, int line);


class Renderer
{
private:
public:
    /* Vertex Buffer is bound in Vertex Array. */
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void Clear() const;
};