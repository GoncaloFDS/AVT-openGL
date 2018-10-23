#include "Mesh.h"

#include "common.h"
#include "vertexBufferLayout.h"
#include <memory>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures) 
	:m_Vertices(vertices), m_Indices(indices), m_Textures(textures){

	SetupMesh();
}

void Mesh::Draw(Shader shader) {
	unsigned int difuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;

	for (unsigned int i = 0; i < m_Textures.size(); i++){
		bool supported = false;
		std::string number;
		std::string name;
		switch (m_Textures[i]->type) {
			case TextureType::Diffuse:
				number = std::to_string(difuseNr++);
				name = "texture_diffuse" + number;
				supported = true;
				break;
			case TextureType::Specular:
				number = std::to_string(specularNr++);
				name = "texture_specular" + number;
				supported = true;
				break;
			case TextureType::Normal:
				number = std::to_string(normalNr++);
				name = "texture_normal" + number;
				supported = true;
				break;
			case TextureType::Mask:
				name = "texture_mask1";
				supported = true;
				break;
		
		}
		shader.Bind();
		if(supported)
			shader.SetUniform1i(name, i);
		m_Textures[i]->Bind(i);
		
	}
	m_VertexArray->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, nullptr));
	m_VertexArray->Unbind();

	glActiveTexture(GL_TEXTURE0);
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
