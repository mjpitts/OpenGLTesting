#include "Renderer.h"
#include <iostream>

/* This function clears openGL error flags, but does not print them. */
void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

/* This function clears the openGL error flags and prints them as it goes. */
bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ") "
            << function << " "
            << file << " : "
            << line << std::endl;
        return false;
    }

    return true;
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
