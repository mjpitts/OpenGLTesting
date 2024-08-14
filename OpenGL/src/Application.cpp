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

/* This struct allows us to return two items from our shader parsing function. */
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType 
    {
        NONE = -1,  VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while(getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos) 
        {
            if (line.find("vertex") != std::string::npos) 
            {
                // Set mode to vertex
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                // Set mode to fragment
                type = ShaderType::FRAGMENT;
            }
            
        }
        else
        {
            /* Prevents indexing errors. */
            if(type != ShaderType::NONE) { ss[(int)type] << line << '\n'; }
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompilerShader(unsigned int type, const std::string& source)
{
   unsigned int id = glCreateShader(type);
   const char* src = source.c_str();
   glShaderSource(id, 1, &src, nullptr);
   glCompileShader(id);

   /* Error handling. */
   int result;
   glGetShaderiv(id, GL_COMPILE_STATUS, &result);
   if (result == GL_FALSE) 
   {
       int length;
       glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
       char* message = (char*)alloca(length * sizeof(char));
       glGetShaderInfoLog(id, length, &length, message);
       std::cout << "Failed to Compile shader " << std::endl;
       std::cout << message << std::endl;

       /* Delete failed shader and return 0. */
       glDeleteShader(id);
       return 0;
   }

   return id;
}

/* Provide openGl with a shader source code and text, want openGl to complie and link, then give unique to bind, link the buffer. */
static unsigned int Createshader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    
    /* Think of these as files that need to be linked. */
    unsigned int vs = CompilerShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompilerShader(GL_FRAGMENT_SHADER, fragmentShader);

    /* Attach shaders to program. */
    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));

    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    /* Get rid of shaders now that they are part of the program. */
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}


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

        /* Vertex array object that binds vertex buffers to their attrib layout. */
        // unsigned int vao;
        // GLCall(glGenVertexArrays(1, &vao));
        // GLCall(glBindVertexArray(vao));

        
        VertexArray va;
        /* Buffer gets bound in constructor so vb.bind() doesn't need to be called. */
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));
        
        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        /* Buffer gets bound in constructor. */
        IndexBuffer ib(indices, 6);

        /* Relative path originates from the working dir. But in viausal studio it can be set to something else. */
        /* In this case the relative path states at the project dir. */
        ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

        unsigned int shader = Createshader(source.VertexSource, source.FragmentSource);
        GLCall((glUseProgram(shader)));

        /* Get uniform id from our target program. */
        GLCall(int location = glGetUniformLocation(shader, "u_Color"));
        /* Uniform locator reurns -1 if not found. */
        ASSERT(location != -1);
        /* Color attribute is vector containing 4 floats, thus we use the 4f uniform. */
        GLCall(glUniform4f(location, 0.5, 0.0, 0.5, 1.0));
        
        /* Loop until the user closes the window */
        /* Red channel for the uniform, and increment that will be used to animate it. */
        float r = 0.5f;
        float increment = 0.0f;
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            GLCall(glUseProgram(shader));

            va.Bind();
            ib.Bind();
            /* Draw call, draws currently bound buffer. Right now that is unsigned int buffer. */
            /* Macro wrapper GlCall takes care of the error handling for us. */
            GLCall(glUniform4f(location, r + std::sin(increment), 0.0, 0.5, 1.0));
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());

            increment += 0.01f;
        }

        /* Clean up shader. */
        GLCall(glDeleteProgram(shader));
    }

    glfwTerminate();
    return 0;
}