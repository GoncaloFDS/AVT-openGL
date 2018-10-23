#pragma once

#include <string>

#include "Common.h"


enum class TextureType {
	NotSupported = 0,
	Diffuse = 1,
	Specular = 2,
	Normal = 3,
	Mask = 4,
};

struct Texture {
	Texture(const std::string& path, const std::string& directory, TextureType type);

	unsigned int id;
	TextureType type;
	std::string path;
	void Bind(unsigned int slot = 0) const;
	void Unbind();
};