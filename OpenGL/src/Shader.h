#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//Set uniforms
	void SetUniform1I(const std::string& name, const int value) const;
	void SetUniform1F(const std::string& name, const float value) const;
	void SetUniform2F(const std::string& name, const glm::vec2& value) const;
	void SetUniform3F(const std::string& name, const glm::vec3& value) const;
	void SetUniform4F(const std::string& name, const glm::vec4& value) const;
	
	void SetUniformMat3(const std::string& name, const glm::mat3& matrix) const;
	void SetUniformMat4(const std::string& name, const glm::mat4& matrix) const;

private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	static unsigned int CompileShader(const unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name) const;

private:
	unsigned int m_RendererId;
	std::string m_FilePath;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;
};

