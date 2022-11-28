#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <vector>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

class ShaderProgram;

struct Vertex final
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texturePosition;

	Vertex(const glm::vec3& position = glm::vec3(0.f),
		   const glm::vec3& normal = glm::vec3(0.f),
		   const glm::vec2& texturePosition = glm::vec2(0.f))
		: position(position), normal(normal), texturePosition(texturePosition) {}
};

class Mesh final
{
private:
	VertexArray vertexArray;
	VertexBuffer vertexCoordsBuffer;
	VertexBuffer colorCoordsBuffer;
	IndexBuffer indexBuffer;

	std::vector<glm::vec3> vertexNormals;
	std::vector<glm::vec2> vertexTexturePositions;
	std::vector<float> vertexPositions;
	
	Renderer::DrawMode drawMode;

	void setupMesh();

	void recalculateVertexNormals();

	std::vector<float>& getVertexPositions();

	std::vector<glm::vec2>& getVertexTexturePositions();

	std::vector<glm::vec3>& getVertexNormals();

public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	Mesh(std::vector<Vertex>& vertices,
		 std::vector<unsigned int>& indices,
		 bool recalculateNormals);
	
	void setDrawMode(Renderer::DrawMode mode) { drawMode = mode; }

	void render(const ShaderProgram* shader, const glm::mat4& modelMatrix);

	void render();
};
