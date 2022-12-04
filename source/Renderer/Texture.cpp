#include "Texture.h"

Texture::Texture(const GLuint width,
				 const GLuint height,
				 const unsigned char* pixels,
				 const unsigned int channels,
				 const GLenum filter,
				 const GLenum wrapMode) : size(glm::uvec2(width, height))
{
	switch (channels)
	{
		case 1:  mode = GL_RED;  break;
		case 4:  mode = GL_RGBA; break;
		case 3:  mode = GL_RGB;  break;
		default: mode = GL_RGBA; break;
	}

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexImage2D(GL_TEXTURE_2D, 0, mode, width, height, 0, mode, GL_UNSIGNED_BYTE, pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture() { glDeleteTextures(1, &id); }

void Texture::bind() const { glBindTexture(GL_TEXTURE_2D, id); }

unsigned int Texture::width() const { return size.x; }

unsigned int Texture::height() const { return size.y; }

const std::string& Texture::getType() const { return type; }
