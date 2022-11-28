#pragma once
#include <functional>
#include <glm/vec3.hpp>

#include "RendererComponent.h"
#include "../../Resources/ResourceManager.h"
#include "../../System/Constants.h"

class Primitive
{
private:
	VertexArray vertexArray;
	VertexBuffer vertexCoordsBuffer;
	VertexBuffer colorCoordsBuffer;
	IndexBuffer indexBuffer;

	GLenum drawMode = GL_TRIANGLES;

public:
	Primitive()
	{
		//	1-----2
		//  |	  |
		//  |	  |
		//  0-----3

		const GLuint indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		const GLfloat vertexCoords[] =
		{
			0.0, 0.0, 1.0,
			0.0, 1.0, 1.0,
			1.0, 1.0, 1.0,
			1.0, 0.0, 1.0
		};

		const GLfloat vertexColors[] =
		{
			1.0, 0.0, 0.0,
			1.0, 0.0, 0.0,
			1.0, 0.0, 0.0,
			1.0, 0.0, 0.0
		};

		vertexCoordsBuffer.init(vertexCoords, sizeof(vertexCoords));
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(3, false);
		vertexArray.addBuffer(vertexCoordsBuffer, vertexCoordsLayout);

		colorCoordsBuffer.init(vertexColors, sizeof(vertexColors));
		VertexBufferLayout vertexColorsLayout;
		vertexColorsLayout.addElementLayoutFloat(3, false);
		vertexArray.addBuffer(colorCoordsBuffer, vertexColorsLayout);

		indexBuffer = IndexBuffer(indices, 6);

		vertexArray.unbind();
		indexBuffer.unbind();
	}

	Primitive(GLuint* indices, int indiciesCount, GLfloat* vertexCoords, int vertexCount, GLfloat* colors, GLenum mode)
	{
		drawMode = mode;
		vertexCoordsBuffer.init(vertexCoords, sizeof(GLfloat) * vertexCount);
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.addElementLayoutFloat(3, false);
		vertexArray.addBuffer(vertexCoordsBuffer, vertexCoordsLayout);

		colorCoordsBuffer.init(colors, sizeof(GLfloat) * vertexCount);
		VertexBufferLayout vertexColorsLayout;
		vertexColorsLayout.addElementLayoutFloat(3, false);
		vertexArray.addBuffer(colorCoordsBuffer, vertexColorsLayout);

		indexBuffer = IndexBuffer(indices, indiciesCount);

		vertexArray.unbind();
		indexBuffer.unbind();
	}

	~Primitive() {}

	Primitive(Primitive&) = delete;
	Primitive operator = (const Primitive&) = delete;

	void render() const;
};

class ShapeRendererComponent : public RendererComponent
{
public:
	ShaderProgram* shader = nullptr;
	Primitive* primitive;

public:

	ShapeRendererComponent() = delete;
	ShapeRendererComponent(const ShapeRendererComponent&) = delete;
	ShapeRendererComponent& operator = (const ShapeRendererComponent&) = delete;

	ShapeRendererComponent(GameObject* gameObject) : RendererComponent(gameObject) {};
	~ShapeRendererComponent() { delete primitive; }

	void render() override;
	void update() override {};

	void deserialize(const nlohmann::json& jsonObject) override;
	void serialize(nlohmann::json& jsonObject) override {} ;
};