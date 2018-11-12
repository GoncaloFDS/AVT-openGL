#include "Mesh.h"

#include "common.h"
#include "vertexBufferLayout.h"
#include <memory>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures)
	:m_Vertices(vertices), m_Indices(indices), m_Textures(textures) {

	SetupMesh();
}

void Mesh::Draw(Shader& shader) {
	BindTextures(shader);

	m_VertexArray->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, nullptr));
	m_VertexArray->Unbind();

}

void Mesh::BindTextures(Shader &shader) {
	unsigned int difuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;

	for (unsigned int i = 0; i < m_Textures.size(); i++) {
		bool supported = false;
		std::string number;
		std::string name;
		switch (m_Textures[i]->type) {
		case TextureType::Diffuse:
			name = "texture_diffuse1";
			supported = true;
			break;
		case TextureType::Specular:
			name = "texture_specular1";
			supported = true;
			break;
		case TextureType::Normal:
			name = "texture_normal1";
			supported = true;
			break;
		case TextureType::Mask:
			name = "texture_mask1";
			supported = true;
			break;

		}
		shader.Bind();
		m_Textures[i]->Bind(i);
		if (supported)
			shader.SetUniform1i(name, i);

	}
}

void Mesh::SetupMesh() {
	m_VertexArray = std::make_unique<VertexArray>();
	m_VertexBuffer = std::make_unique<VertexBuffer>(&m_Vertices[0], m_Vertices.size() * sizeof(Vertex));
	VertexBufferLayout layout;
	layout.Push<float>(3); // vertex Positions
	layout.Push<float>(3); // vertex normals
	layout.Push<float>(2); // vertex texture coords
	layout.Push<float>(3); // vertex tangent
	layout.Push<float>(3); // vertex bitangent
	m_VertexArray->AddBuffer(*m_VertexBuffer, layout);
	m_IndexBuffer = std::make_unique<IndexBuffer>(&m_Indices[0], m_Indices.size());
}
