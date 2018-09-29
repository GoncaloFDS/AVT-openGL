#pragma once

#include <string>

#include "Common.h"


// class Texture {
// private:
// 	unsigned int m_RendererID;
// 	std::string m_Type;
// 	std::string m_FilePath;
// 	std::string m_Name;
// 	unsigned char* m_LocalBuffer;
// 	int m_Width, m_Height, m_BPP; // bits per pixel
// 
// public:
// 	Texture(const std::string& path, const std::string&);
// 	~Texture();
// 
// 	void Bind(unsigned int slot = 0) const;
// 	void Unbind();
// 
// 	int GetWidth() const;
// 	int GetHeight() const;
// 	const std::string& GetFilePath();
// 	const std::string& GetType();
// 	const std::string& GetName();
// 	unsigned int ID() {
// 		return m_RendererID;
// 	}
// 	
// };

struct Texture {
	Texture(const std::string& path, const std::string& directory, const std::string& type);

	unsigned int id;
	std::string type;
	std::string path;
};