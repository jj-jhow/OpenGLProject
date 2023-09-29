#pragma once
#include <string>
#include <GL/glew.h>

class TextureLoader 
{
public:
	TextureLoader();
	~TextureLoader();

	GLuint getTexture(std::string _textureFileName);
};

