#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "Renderer.h"

Shader::Shader(const std::string& filepath)
	:m_filepath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);

    /* Show the Shader Info: */
    std::cout << '\n' << "Vertex Info:" << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "Fragment Info:" << std::endl;
    std::cout << source.FragmentSource << std::endl;
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}


// In order to reduce GPUs complicated and replaced Shader creating
ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    // Input the filepath
    std::ifstream stream(filepath);

    // choose Vertex or Fragment
    enum class ShaderType
    {
        NONE        = -1,
        VERTEX      =  0,
        FRAGMENT    =  1
    };

    // ss[] is to distinguish the string::Vertex and the string::Fragment
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }

        else if (type != ShaderType::NONE)
        {
            // Not the #shader vertex or #shader fragment
            // Input the fileline + '\n' directly.
            ss[(int)type] << line << '\n';
        }
    }

    // Eventually retrun the contant of file
    return { ss[0].str(), ss[1].str() };

}

/* TODO: create the Shader Complier (link the shader) */
unsigned int Shader::ComplieShader(unsigned int type, const std::string& source)
{
    // Create the Shader
    unsigned int id = glCreateShader(type);

    // Get the first address of Source 
    // source.c_str() == source[0]
    const char* src = source.c_str();

    // Get the shaderSource (Once per compile)
    glShaderSource(id, 1, &src, nullptr);

    // Compile:
    glCompileShader(id);


    // TODO: Error handing.
    // result is to save the address of Shaderiv
    int result;

    // glGetShaderiv() (make the mistakes(Integer) output to the &result)
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        // save Info
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        // show Info
        // To create a varity Stack:
        char* message = (char*)malloc(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);

        // Cout the mistake messages
        std::cout << "Fail to Compile " << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << std::endl;
        std::cout << message << std::endl;

        // Dont forget to delete the ERROR shader:
        glDeleteShader(id);

        return 0;
    }


    return id;
}

/* TODO: create the Vertex Shader and Fragment Shader */
unsigned int Shader::CreateShader(const std::string& VertexShader, const std::string& FragmentShader)
{
    // Create the start program and complie the V and F shader
    unsigned int program = glCreateProgram();
    unsigned int vs = ComplieShader(GL_VERTEX_SHADER, VertexShader);
    unsigned int fs = ComplieShader(GL_FRAGMENT_SHADER, FragmentShader);

    // Get attach the program and the Shader:
    // By the way: Link and ValidateLink (By program)
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    return program;
}

        /* Bind the program and the Shader */
        // unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        // glUseProgram(shader);

        /* Bind the shader and the Uniform */
        // (int location = glGetUniformLocation(shader, "u_Color"));
        // ASSERT(location != -1);
        // GLCall(glUniform4f(location, 0.8f, 0.2f, 0.8f, 1.0f));

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning Uniform '" << name << "' doesn't exist!!" << std::endl;

    m_UniformLocationCache[name] = location;
    return location;
}