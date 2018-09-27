#include "mesh.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include "Common.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

Mesh::Mesh(const std::string& path) {
  LoadFromFile(path);
  CreateBuffers();
}

Mesh::~Mesh() {
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
  GLCall(glBindVertexArray(0));
}

bool Mesh::LoadFromFile(const std::string& path) {
  std::ifstream file = std::ifstream(path);
  if (file.fail()) {
    LOG("Failed to load Mesh file: " << path);
    ASSERT(false);
  }
  std::string line;
  std::string t;
  while (std::getline(file, line)) {
    std::stringstream ss = std::stringstream(line);
    ss >> t;
    if (t == "v") {
      glm::vec3 v;
      ss >> v.x >> v.y >> v.z;
      m_Vertices.push_back(v);
    } else if (t == "vt") {
      glm::vec2 uv;
      ss >> uv.x >> uv.y;
      m_Uvs.push_back(uv);
    } else if (t == "vn") {
      glm::vec3 vn;
      ss >> vn.x >> vn.y >> vn.z;
      m_Normals.push_back(vn);
    } else if (t == "f") {
      std::string token;
      for (int i = 0; i < 3; i++) {
        std::getline(ss, token, '/');
        if (!token.empty()) 
			m_VertexIndices.push_back(std::stoi(token) - 1);
        std::getline(ss, token, '/');
        if (!token.empty())
			m_UvIndices.push_back(std::stoi(token) - 1);
        std::getline(ss, token, ' ');
        if (!token.empty()) 
			m_NormalIndices.push_back(std::stoi(token) - 1);
      }
    }
  }
  file.close();
  return true;
}

void Mesh::CreateBuffers() {
  m_Va = new VertexArray();
  auto m_Vb = new VertexBuffer(&m_Vertices[0], m_Vertices.size() * 3 * sizeof(float));
  VertexBufferLayout layout;
  layout.Push<float>(3);  // xyz
  m_Va->AddBuffer(*m_Vb, layout);
  m_Ib = new IndexBuffer(&m_VertexIndices[0], m_VertexIndices.size());
}

void Mesh::Draw() {
  m_Va->Bind();
  m_Ib->Bind();
  GLCall(glDrawElements(GL_TRIANGLES, m_Ib->GetCount(), GL_UNSIGNED_INT, nullptr));
}
