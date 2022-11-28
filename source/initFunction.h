#pragma once
#define _USE_MATH_DEFINES

#include <vector>
#include <math.h>

#include "./Renderer/Mesh.h"
#include "./Renderer/Renderer.h"
#include "./Resources/ResourceManager.h"


void generateCubeModel()
{
	int vertexCount = 8;
	int indicesCount = 3 * 2 * 6;

	std::vector<glm::vec3> points = {
		glm::vec3(-1.f, -1.f, -1.f),
		glm::vec3( 1.f, -1.f, -1.f),
		glm::vec3( 1.f,  1.f, -1.f),
		glm::vec3(-1.f,  1.f, -1.f),
		glm::vec3(-1.f,  1.f,  1.f),
		glm::vec3(-1.f, -1.f,  1.f),
		glm::vec3( 1.f,  1.f,  1.f),
		glm::vec3( 1.f, -1.f,  1.f)
	};

	std::vector<unsigned int> indicies = {
		0, 1, 3,
		3, 2, 1,
		1, 2, 6,
		6, 7, 1,
		7, 1, 0,
		0, 5, 7,
		0, 3, 4,
		4, 0, 5,
		5, 7, 6,
		6, 5, 4,
		4, 3, 6,
		6, 2, 3
	};

	std::vector<Vertex> verticies;
	for (unsigned int i = 0; i < vertexCount; ++i)
	{
		verticies.push_back(Vertex(points[i], glm::vec3(), glm::vec2()));
	}

	std::vector<Mesh*> meshes;
	meshes.push_back(new Mesh(verticies, indicies, true));

	ResourceManager::addModel("cube", meshes, Renderer::DrawMode::Triangles);
}

void initLabResources()
{
	generateCubeModel();
}