#include "Shader.h"

#include <GL/glew.h>

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string& filepath)
	: m_RendererId(0), m_FilePath(filepath)
{
    const auto source = ParseShader(filepath);
    m_RendererId = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererId));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        None = -1, Vertex = 0, Fragment = 1
    };

    std::string line;
    std::stringstream ss[2];
    auto type = ShaderType::None;

    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::Vertex;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::Fragment;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(const unsigned int type, const std::string& source)
{
    GLCall(const unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        const auto message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!\n";
        std::cout << message << "\n";
        GLCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCall(const unsigned int program = glCreateProgram());
    GLCall(const unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
    GLCall(const unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererId));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1I(const std::string& name, const int value) const
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1F(const std::string& name, const float value) const
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform2F(const std::string& name, const glm::vec2& value) const
{
    GLCall(glUniform2f(GetUniformLocation(name), value.x, value.y));
}

void Shader::SetUniform3F(const std::string& name, const glm::vec3& value) const
{
    GLCall(glUniform3f(GetUniformLocation(name), value.x, value.y, value.z));
}

void Shader::SetUniform4F(const std::string& name, const glm::vec4& value) const
{
    GLCall(glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w));
}

void Shader::SetUniformMat3(const std::string& name, const glm::mat3& matrix) const
{
    GLCall(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

void Shader::SetUniformMat4(const std::string& name, const glm::mat4& matrix) const
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

int Shader::GetUniformLocation(const std::string& name) const
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];
	
    GLCall(const int location = glGetUniformLocation(m_RendererId, name.c_str()));
	
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!\n";
	
    m_UniformLocationCache[name] = location;
	return location;
}
