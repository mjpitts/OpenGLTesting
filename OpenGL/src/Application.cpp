#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

// Episode 7: Writing shaders

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
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    /* Get rid of shaders now that they are part of the program. */
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
// Episode 7 -----------------------


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


    // Episode 4: define buffer and buffer data ----------------------------------
    /* openGl operates as a state machine, everthing generated will have a unique ID, an integer. */

    float positions[6] = {
        -0.5, -0.5,
         0.5, -0.5,
         0.0,  0.5
    };

    /* Vector buffer ID container that will be passed to buffer generation. */
    unsigned int buffer;
    /* Generate buffer that openGL will draw from and assigns ID to the unsigned int address. */
    glGenBuffers(1, &buffer);
    /* Set use of Buffer and bind buffer to ID. */
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    /* Docs.gl will tell you more about buffer data. */
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);
    // Episode 4: --------------------------------------
    
    // Episode 7 ---------------------

    /* Shader source code. */
    std::string vertexShader = 
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(0.5, 0.0, 0.5, 1.0);\n"
        "}\n";

    unsigned int shader = Createshader(vertexShader, fragmentShader);
    glUseProgram(shader);

    // Episode 7 ---------------------

    // Episode 5: Vertex attributes --------------------

    /* Define vertex position attribute. */
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    /* Enable our vertex attribute, which is positions ni this case. */
    glEnableVertexAttribArray(0);
    // Episode 5 --------------------------------------

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Draw call, draws currently bound buffer. Right now that is unsigned int buffer. */
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    /* Clean up shader. */
    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}