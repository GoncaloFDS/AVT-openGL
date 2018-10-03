#include "Mesh.h"

#include "common.h"
#include "vertexBufferLayout.h"
#include <memory>

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) 
	:m_Vertices(vertices), m_Indices(indices), m_Textures(textures){

	SetupMesh();
}

void Mesh::Draw(Shader shader) {
	unsigned int difuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	for (unsigned int i = 0; i < m_Textures.size(); i++){
		
		std::string number;
		std::string name = m_Textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(difuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); 
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); 
		else if (name == "texture_height")
			number = std::to_string(heightNr++);

		shader.Bind();
		shader.SetUniform1i(name + number, i);
		
		m_Textures[i].Bind(i);
		
	}
	m_VertexArray->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, nullptr));
	m_VertexArray->Unbind();

	// always good practice to set everything back to defaults once configured.
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
