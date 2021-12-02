#pragma once
#include <string>

class Texture {
	unsigned int m_textureId;
public:
	int width, height, channels;

	Texture(const std::string& path);
	Texture(unsigned char* data, int channels, int width, int height);

	void Bind();
	void UnBind();
	void BindTo(int location);
	void UnbindFrom(int location);
};