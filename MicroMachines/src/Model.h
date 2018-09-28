#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"

class Model {
public:
	Model(const std::string& filePath);


	void Draw(Shader shader);

private:
	std::vector<Mesh> m_Meshes;
	std::vector<Texture> m_Textures;
	std::string m_Directory;
	bool gammaCorrection;

	void LoadModel(const std::string& filePath);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type,const std::string& typeName);
};

