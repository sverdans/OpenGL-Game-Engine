#pragma once
#include <string>

#include "RendererComponent.h"

class Model;
class ShaderProgram;

class ModelRendererComponent : public RendererComponent
{
private:
	Renderer::DrawMode drawMode;

	const Model* model = nullptr;
	const ShaderProgram* shader = nullptr;

	bool renderWithTexture;
	glm::vec3 color;

public:
	ModelRendererComponent() = delete;
	ModelRendererComponent(const ModelRendererComponent&) = delete;
	ModelRendererComponent& operator = (const ModelRendererComponent&) = delete;
	~ModelRendererComponent() = default;

	ModelRendererComponent(GameObject * gameObject);

	void setModel(const Model* model);
	void setShader(const ShaderProgram* shader);

	void render() override;

	void deserialize(const nlohmann::json& jsonObject) override;
	void serialize(nlohmann::json& jsonObject) override;
};