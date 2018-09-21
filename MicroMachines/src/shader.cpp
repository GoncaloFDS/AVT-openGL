#include "shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "common.h"


Shader::Shader(const std::string& filepath)
    : m_FilePath(filepath), m_RendererID(0) {
  ShaderProgramSource source = ParseShader(filepath);
  m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader() {}

void Shader::Bind() const { 
	GLCall(glUseProgram(m_RendererID)); 
}

void Shader::Unbind() const { 
	GLCall(glUseProgram(0)); 
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix) {
  GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
  std::ifstream vertexStream(filepath + ".vert");

  std::string line;
  std::stringstream ss[2];

  while (getline(vertexStream, line)){
    ss[0] << line << '\n';
  }
  
  std::ifstream fragmentStream(filepath + ".frag");

  while (getline(fragmentStream, line)) {
    ss[1] << line << '\n';
  }

  return {ss[0].str(), ss[1].str()};
}


unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  GLCall(glAttachShader(program, vs));
  GLCall(glAttachShader(program, fs));
  GLCall(glLinkProgram(program));
  GLCall(glValidateProgram(program));

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
  unsigned int id = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE) {
    int lenght;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &lenght);
    auto message = (char*)alloca(lenght * sizeof(char));  // dynamically allocate, on the stack, a string with error size

    glGetShaderInfoLog(id, lenght, &lenght, message);
    std::cout << "Failed to Compile " << (type == GL_VERTEX_SHADER ? "Vertex Shader" : "Fragment Shader") << std::endl;
    std::cout << message << std::endl;
    glDeleteShader(id);
    return 0;
  }

  return id;
}

int Shader::GetUniformLocation(const std::string& name) {
  if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    return m_UniformLocationCache[name];

  GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
  if (location == -1)
    std::cout << "Warning: uniform '" << name << "' doesn't exist!"
              << std::endl;

  m_UniformLocationCache[name] = location;
  return location;
}

