#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError;\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

/* This function clears openGL error flags, but does not print them. */
static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

/* This function clears the openGL error flags and prints them as it goes. */
static bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error =glGetError())
    {
        std::cout << "[OpenGL Error] (" << error << ") " 
            << function << " " 
            << file << " : " 
            << line << std::endl;
        return false;
    }

    return true;
}

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

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK)
        std::cout << "glew init error" << std::endl;

    /* Print openGl version */
    std::cout << glGetString(GL_VERSION) << std::endl;

    /* OpenGl operates as a state machine, everthing generated will have a unique ID, an integer. */
    /* Drawing square counter-clockwise. */
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

    /* Vector buffer ID container that will be passed to buffer generation. */
    unsigned int buffer;
    /* Generate buffer that openGL will draw from and assigns ID to the unsigned int address. */
    GLCall(glGenBuffers(1, &buffer));
    /* Set use of Buffer and bind buffer to ID. */
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    /* Docs.gl will tell you more about buffer data. */
    GLCall(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

    /* Same as vectex buffer work, but for index buffer. */
    unsigned int ibo;
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6  * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    /* Relative path originates from the working dir. But in viausal studio it can be set to something else. */
    /* In this case the relative path states at the project dir. */
    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

    unsigned int shader = Createshader(source.VertexSource, source.FragmentSource);
    GLCall((glUseProgram(shader)));

    /* Define vertex position attribute. */
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
    /* Enable our vertex attribute, which is positions ni this case. */
    GLCall(glEnableVertexAttribArray(0));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Draw call, draws currently bound buffer. Right now that is unsigned int buffer. */
        /* Macro wrapper GlCall takes care of the error handling for us. */
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        /* Swap front and back buffers */
        GLCall(glfwSwapBuffers(window));

        /* Poll for and process events */
        glfwPollEvents();
    }

    /* Clean up shader. */
    GLCall(glDeleteProgram(shader));

    glfwTerminate();
    return 0;
}