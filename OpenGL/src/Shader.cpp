#include "Shader.h"
#include "Renderer.h"


Shader::Shader(const std::string& filepath)
	:m_Filepath(filepath), m_RenderID(0)
{

    ShaderProgramSource source = ParseShader(filepath);
    m_RenderID = Createshader(source.VertexSource, source.FragmentSource);
    
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RenderID));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
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
            if (type != ShaderType::NONE) { ss[(int)type] << line << '\n'; }
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompilerShader(unsigned int type, const std::string& source)
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
unsigned int Shader::Createshader(const std::string& vertexShader, const std::string& fragmentShader)
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


void Shader::Bind() const
{
    GLCall(glUseProgram(m_RenderID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (M_UniformLocationCache.find(name) != M_UniformLocationCache.end()) 
    {
        return M_UniformLocationCache[name];
    }

    GLCall(int location = glGetUniformLocation(m_RenderID, name.c_str()));
    if (location == -1)
    {
        std::cout << "Warning: uniform " << name << " doesn't Exist!" << std::endl;
    }

    M_UniformLocationCache[name] = location;
    return location;
}