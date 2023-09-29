#pragma once
#include <string>
#include <map>

#include <GL/glew.h>
#include "Dependencies/glm/glm/glm.hpp"
#include "Dependencies/glm/glm/gtc/matrix_transform.hpp"
#include "Dependencies/glm/glm/gtc/type_ptr.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character
{
	GLuint		TextureID;	// Texture ID of each glyph texture
	glm::ivec2	Size;		// glyph Size
	glm::ivec2	Bearing;	// baseline to left/top of glyph
	GLuint		Advance;	// id to next glyph
};

class TextRenderer
{
public:
	TextRenderer(std::string _text, std::string _font, int _size, glm::vec3 _color, GLuint _program);
	~TextRenderer();

	void Draw();

	void setPosition(glm::vec2 _position);
	void setText(std::string _text);

private:
	std::string text;

	std::map<GLchar, Character> characters;
	
	glm::vec3 color;
	glm::vec2 position;

	GLfloat scale;
	
	GLuint vao;
	GLuint vbo;
	GLuint program;
};

