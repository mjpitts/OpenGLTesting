#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Makes openGL use the core version of the software as to not provide a default vertex array. */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* sync window and monitor refresh rate. */
    glfwSwapInterval(1);

    if(glewInit() != GLEW_OK)
        std::cout << "glew init error" << std::endl;

    /* Print openGl version */
    std::cout << glGetString(GL_VERSION) << std::endl;

    /* Drawing square counter-clockwise. */
    {
        float positions[] = {
            -0.5, -0.5, 0.0f, 0.0f, // 0 
             0.5, -0.5, 1.0f, 0.0f,// 1
             0.5,  0.5, 1.0f, 1.0f,// 2
            -0.5,  0.5, 0.0f, 1.0f,// 3
        };

        /* Triangle 1: 0, 1, 2 Triangle 2: 2, 3, 0 */
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        
        
        /* Buffer gets bound in constructor so vb.bind() doesn't need to be called. */
        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        /* Buffer gets bound in constructor. */
        IndexBuffer ib(indices, 6);

        /* Projection matrix to fix aspect ratio problem. */
        glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -0.75f, 0.75f, -0.5f, 0.5f);

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        shader.SetUniform4f("u_Color", 0.5, 0.0, 0.5, 1.0);

        Texture texture("res/textures/skel.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);
        shader.SetUniformMat4f("u_MVP", proj);
        
        Renderer renderer;

        float r = 0.5f;
        float increment = 0.0f;

        /* Loop until user closes window.*/
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            shader.SetUniform4f("u_Color", r + std::sin(increment), 0.0, 0.5, 1.0);
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            renderer.Draw(va, ib, shader);

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());

            increment += 0.01f;
        }

        /* Clean up shader. */
        shader.Unbind();
    }

    glfwTerminate();
    return 0;
}