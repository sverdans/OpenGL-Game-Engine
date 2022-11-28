#pragma once
#include "ShapeRendererComponent.h"
#include "TransformComponent.h"

#include "../GameObject.h"
#include "../ObjectsManager.h"
#include "../../Renderer/Renderer.h"

void Primitive::render() const
{
	Renderer::draw(vertexArray, indexBuffer, static_cast<Renderer::DrawMode>(drawMode));
}

void ShapeRendererComponent::render()
{
	if (!renderEnable)
		return;

	shader->use();
	shader->setMatrix("modelMat", gameObject->getComponent<TransformComponent>()->getModelMatrix());
	primitive->render();
	shader->unuse();
}

void ShapeRendererComponent::deserialize(const nlohmann::json& jsonObject)
{
	renderEnable = jsonObject["renderEnable"];
	shader = ResourceManager::getShader(jsonObject["Shader"]);

	primitive = new Primitive();
}
