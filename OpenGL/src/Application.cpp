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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
    window = glfwCreateWindow(1280, 960, "Hello World", NULL, NULL);
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
        glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.50f, 1.50f, -0.5f, 0.5f);
        /* View matrix which mimics camera actions. */
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-1, 0, 0));

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();

        shader.SetUniform4f("u_Color", 0.5, 0.0, 0.5, 1.0);

        Texture texture("res/textures/skel.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);
        
        Renderer renderer;

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");

        glm::vec3 translation(1.0, 0.0, 0.0);


        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        /* Loop until user closes window.*/
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
            /* Resulting matrix which represent all the positioning in our scene.
            Multiplication order is dependant on how the matrix data is stored in different frameworks. */
            glm::mat4 mvp = proj * view * model;

            shader.SetUniform4f("u_Color", 0.5, 0.0, 0.5, 1.0);
            shader.SetUniformMat4f("u_MVP", mvp);

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            renderer.Draw(va, ib, shader);


            // ImGui Window.
            {
                ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 4.0f);            
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }

        /* Clean up shader. */
        shader.Unbind();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}