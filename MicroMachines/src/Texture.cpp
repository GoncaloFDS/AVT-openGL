#include "Texture.h"

#include "stb_image/stb_image.h"
#include "Common.h"

Texture::Texture(const std::string& path, const std::string& type)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), 
		m_Width(0), m_Height(0), m_BPP(0), m_Type(type){

	m_Name = path.substr(path.find_last_of('/')+1, path.size());

	stbi_set_flip_vertically_on_load(1); //opengl expects texture to be flipped on the y axis
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);
}

Texture::~Texture() {
	GLCall(glDeleteTextures(1, &m_RendererID));
}


void Texture::Bind(unsigned int slot) const {
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() {
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

int Texture::GetWidth() const {
	return m_Width;
}

int Texture::GetHeight() const {
	return m_Height;
}

const std::string& Texture::GetFilePath() {
	return m_FilePath;
}

const std::string& Texture::GetType() {
	return m_Type;
}

const std::string& Texture::GetName() {
	return m_Name;
}
