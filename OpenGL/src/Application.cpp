#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Meaks openGL use the core version of the software as to not provide a default vertex array. */
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

    /* OpenGl operates as a state machine, everthing generated will have a unique ID, an integer. */
    /* Drawing square counter-clockwise. */
    {
        float positions[] = {
            -0.5, -0.5, // 0 
             0.5, -0.5, // 2
             0.5,  0.5, // 3
            -0.5,  0.5, // 4
        };

        // Define how we want out positions drawn so we do have to redraw indices
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };
        
        VertexArray va;
        /* Buffer gets bound in constructor so vb.bind() doesn't need to be called. */
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));
        
        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        /* Buffer gets bound in constructor. */
        IndexBuffer ib(indices, 6);

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        shader.SetUniform4f("u_Color", 0.5, 0.0, 0.5, 1.0);
        
        /* Loop until the user closes the window */
        /* Red channel for the uniform, and increment that will be used to animate it. */
        float r = 0.5f;
        float increment = 0.0f;
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            va.Bind();
            ib.Bind();

            shader.SetUniform4f("u_Color", r + std::sin(increment), 0.0, 0.5, 1.0);
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

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