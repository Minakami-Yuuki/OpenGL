#pragma once

#include <string>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	unsigned int m_RendererID;
	std::string m_filepath;

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// set Uniforms
	void SetUniform4f(const std::string name, float v0, float v1, float v2, float v3);

private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int ComplieShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& VertexShader, const std::string& FragmentShader);
	unsigned int GetUniformLocation(const std::string& name);
};