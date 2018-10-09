#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "SceneNode.h"
#include "Mesh.h"
#include "common.h"

class Model : public SceneNode {
public:
	Model(const std::string& name, const std::string& filePath);

	virtual void OnRender() override;
	//virtual void OnUpdate() override;

	void SetShader(Shader& shader);

	void Translate(Direction dir, float amt = 1.0f);

private:
	std::vector<Mesh> m_Meshes;
	std::vector<Texture> m_Textures;
	std::string m_Directory;
	Shader* m_Shader;

	void LoadModel(const std::string& filePath);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
};

