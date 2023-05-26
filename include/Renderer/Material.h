#pragma once
#include <glm/vec3.hpp>

class Texture;

class Material
{
public:
	friend class ResourceManager;

private:
	Material() {}
	~Material() {}

	Material(const Material&) = delete;
	Material& operator = (const Material&) = delete;

public:
	glm::vec3 color = glm::vec3(0.f, 0.f, 0.f);
	std::vector<Texture*> textures;
};