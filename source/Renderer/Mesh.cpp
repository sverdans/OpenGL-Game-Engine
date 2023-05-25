#include "Mesh.h"

#include "ShaderProgram.h"
#include "Material.h"
#include "Texture.h"


Mesh::Mesh(std::vector<Vertex>& vertices,
	std::vector<unsigned int>& indices,
	Material* material,
	bool recalculateNormals = true)
	: vertices(vertices), indices(indices), material(material)
{
	if (recalculateNormals)
		recalculateVertexNormals();

	getVertexNormals();
	getVertexPositions();
	getVertexTexturePositions();

	setupMesh();
}

void Mesh::setupMesh()
{
	vertexCoordsBuffer.init(&vertexPositions.front(), vertexPositions.size() * sizeof(float));
	VertexBufferLayout vertexCoordsLayout;
	vertexCoordsLayout.addElementLayoutFloat(3, false);
	vertexArray.addBuffer(vertexCoordsBuffer, vertexCoordsLayout);

	normalsBuffer.init(&vertexNormals.front(), vertexNormals.size() * sizeof(glm::vec3));
	VertexBufferLayout normalsLayout;
	normalsLayout.addElementLayoutFloat(3, false);
	vertexArray.addBuffer(normalsBuffer, normalsLayout);

	textureCoordsBuffer.init(&vertexTexturePositions.front(), vertexTexturePositions.size() * sizeof(glm::vec2));
	VertexBufferLayout textureCoordsLayout;
	textureCoordsLayout.addElementLayoutFloat(2, false);
	vertexArray.addBuffer(textureCoordsBuffer, textureCoordsLayout);

	indexBuffer = IndexBuffer(&indices.front(), indices.size());

	vertexArray.unbind();
	indexBuffer.unbind();
}

void Mesh::recalculateVertexNormals()
{
	std::vector<glm::vec3> faceNormals;
	for (int i = 0; i < indices.size(); i += 3)
	{
		glm::vec3 a(vertices[indices[i]].position.x,
					vertices[indices[i]].position.y,
					vertices[indices[i]].position.z);

		glm::vec3 b(vertices[indices[i + 1]].position.x,
					vertices[indices[i + 1]].position.y,
					vertices[indices[i + 1]].position.z);

		glm::vec3 c(vertices[indices[i + 2]].position.x,
					vertices[indices[i + 2]].position.y,
					vertices[indices[i + 2]].position.z);

		glm::vec3 n = glm::normalize(glm::cross(b - a, c - a));

		faceNormals.push_back(n);
	}

	for (int i = 0; i < vertices.size(); i++)
	{
		std::vector<int> trianglesUsingThisVertex;
		for (int j = 0; j < indices.size(); j += 3)
			for (int k = 0; k < 3; k++)
				if (i == indices[j + k])
				{
					trianglesUsingThisVertex.push_back(j / 3);
					break;
				}

		glm::vec3 n = glm::vec3();
		for (int j = 0; j < trianglesUsingThisVertex.size(); j++)
		{
			n += faceNormals[trianglesUsingThisVertex[j]];
		}
		vertices[i].normal = glm::normalize(n);
	}
}

std::vector<float>& Mesh::getVertexPositions()
{
	vertexPositions.resize(vertices.size() * 3);

	unsigned int i = 0;
	for (auto& it : vertices)
	{
		vertexPositions[i++] = it.position.x;
		vertexPositions[i++] = it.position.y;
		vertexPositions[i++] = it.position.z;
	}

	return vertexPositions;
}

std::vector<glm::vec2>& Mesh::getVertexTexturePositions()
{
	vertexTexturePositions.resize(vertices.size());

	unsigned int i = 0;
	for (auto& n : vertices)
		vertexTexturePositions[i++] = n.texturePosition;

	return vertexTexturePositions;
}

std::vector<glm::vec3>& Mesh::getVertexNormals()
{
	vertexNormals.resize(vertices.size());

	unsigned int i = 0;
	for (auto& it : vertices)
		vertexNormals[i++] = it.normal;

	return vertexNormals;
}

void Mesh::render(const ShaderProgram* shader)
{
	unsigned int diffuseIndex = 1;
	unsigned int specularIndex = 1;

	for (unsigned int i = 0; i < material->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		
		std::string type = material->textures[i]->getType();
		std::string number;

		if (type == "diffuseTexture")
			number = std::to_string(diffuseIndex++);
		else if (type == "specularTexture")
			number = std::to_string(specularIndex++);

		material->textures[i]->bind();
		shader->setInt((type + number).c_str(), i);
	}
	glActiveTexture(GL_TEXTURE0);

	if (material)
	{
		shader->setVec3("materialColor", material->color);
		shader->setInt("useTexture", material->textures.size() != 0);
	}

	Renderer::draw(vertexArray, indexBuffer, drawMode);
}
