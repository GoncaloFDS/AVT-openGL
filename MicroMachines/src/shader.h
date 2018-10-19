#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader {
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;

public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;
  
	//Set Uniforms
	void SetUniform1i(const std::string& name, int v0);
	void SetUniform1f(const std::string& name, float v0);
	void SetUniform3fv(const std::string& name, glm::vec3 v);
	void SetUniform4fv(const std::string& name, glm::vec4 v);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	void SetUniformMat3f(const std::string& name, const glm::mat3& matrix);

private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	int GetUniformLocation(const std::string& name);
};
