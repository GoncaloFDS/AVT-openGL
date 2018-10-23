#include "Model.h"
#include "common.h"
#include "SceneNode.h"
#include "Texture.h"


Model::Model(const std::string& filePath) {
	LoadModel(filePath);
}

void Model::Draw(Shader& shader)
{
	for (auto& mesh : m_Meshes)
		mesh.Draw(shader);
}

void Model::LoadModel(const std::string& filePath) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs); 
	// we flip the UVs cause opengl loads them y inverted

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		LOG("Error::ASSIMP::" << importer.GetErrorString());
		ASSERT(false);
	}
	m_Directory = filePath.substr(0, filePath.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_Meshes.push_back(ProcessMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture*> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		glm::vec3 vector;

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Position = vector;
		// normals
		if (mesh->HasNormals()) {
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
		}
		// texture coordinates
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
			// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		// tangent
		if (mesh->HasTangentsAndBitangents()) {

			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;

			// bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;
		}
		vertices.push_back(vertex);
	}

	// now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		// retrieve all indices of the face and store them in the indices vector
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// process materials
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	// Same applies to other texture as the following list summarizes:
	// diffuse: texture_diffuseN
	// specular: texture_specularN
	// normal: texture_normalN

	// 1. diffuse maps
	std::vector<Texture*> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE);
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	// 2. specular maps
	std::vector<Texture*> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR);
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	// 3. normal maps
	std::vector<Texture*> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT);
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	// 3. ambient maps
	std::vector<Texture*> ambientMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT);
	textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());

	// return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, textures);
}

std::vector<Texture*> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type) {
	std::vector<Texture*> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (auto texture : m_Textures) {
			if (std::strcmp(texture->path.data(), str.C_Str()) == 0) {
				textures.push_back(texture);
				skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip) {   // if texture hasn't been loaded already, load it
			TextureType tType;
			switch (type) {
				case aiTextureType_DIFFUSE:
					tType = TextureType::Diffuse;
					break;
				case aiTextureType_SPECULAR:
					tType = TextureType::Specular;
					break;
				case aiTextureType_HEIGHT: //TODO
					tType = TextureType::Normal;
					break;
				case  aiTextureType_AMBIENT: // Actually used as a mask
					tType = TextureType::Mask;
					break;

			}
			auto texture = new Texture(str.C_Str(), m_Directory, tType);
			textures.push_back(texture);
			m_Textures.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

void Model::BindTextures(Shader &shader) {
	for(auto& mesh : m_Meshes)
		mesh.BindTextures(shader);
}

