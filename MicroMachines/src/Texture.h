#pragma once

#include <string>

#include "Common.h"



enum class TextureType {
	NotSupported = 0,
	Diffuse = 1,
	Specular = 2,
	Normal = 3,
	Mask = 4,
	Other = 5
};

struct Texture {
	unsigned int id;
	TextureType type;
	std::string path;

	Texture(const std::string& path, const std::string& directory, TextureType type);
	Texture(glm::vec2 size, TextureType type = TextureType::Other);

	void Bind(unsigned int slot = 0) const;
	void Unbind();
};