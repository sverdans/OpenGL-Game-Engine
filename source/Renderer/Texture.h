#pragma once
#include <iostream>
#include <string>
#include <map>
#include <glad/glad.h>
#include <glm/vec2.hpp>

class Texture
{
public:
	friend class ResourceManager;

private:
	GLuint id;
	GLint mode;
	std::string type;

	glm::uvec2 size;

	Texture(const GLuint width,
			const GLuint height,
			const unsigned char* pixels,
			const unsigned int channels,
			const GLenum filter,
			const GLenum wrapMode);

	~Texture();

	Texture() = delete;
	Texture(const Texture&) = delete;
	Texture& operator = (const Texture&) = delete;

public:

	void bind() const;
	unsigned int width() const;
	unsigned int height() const;
	const std::string& getType() const;
};