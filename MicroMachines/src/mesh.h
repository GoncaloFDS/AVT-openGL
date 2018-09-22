#pragma once
#include <string>
#include <vector>

#include "glm/glm.hpp"

class VertexArray;
class VertexBuffer;
class IndexBuffer;

class Mesh {
 private:
  std::vector<unsigned int> m_VertexIndices, m_UvIndices, m_NormalIndices;
  std::vector<glm::vec3> m_Vertices;
  std::vector<glm::vec2> m_Uvs;
  std::vector<glm::vec3> m_Normals;

  VertexArray* m_Va;
  VertexBuffer* m_Vb;
  IndexBuffer* m_Ib;

 public:
  Mesh(const std::string& path);
  ~Mesh();
  void Draw();

 private:
  bool LoadFromFile(const std::string& path);
  void CreateBuffers();
};
