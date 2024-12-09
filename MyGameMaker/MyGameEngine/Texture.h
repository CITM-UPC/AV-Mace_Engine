#pragma once

#include <string>

using namespace std;

class Texture
{
private:
	unsigned int m_RendererID;
	string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;

public:
	Texture() = default;
	Texture(const string& path, unsigned int* valor = nullptr);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	string GetFilePath() const { return m_FilePath; }
};