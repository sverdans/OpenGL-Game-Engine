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

	bool useTexture;
	glm::vec3 color;

	float specularStrength;
	int specularity;

public:
	ModelRendererComponent() = delete;
	ModelRendererComponent(const ModelRendererComponent&) = delete;
	ModelRendererComponent& operator = (const ModelRendererComponent&) = delete;
	~ModelRendererComponent() = default;

	ModelRendererComponent(GameObject * gameObject);

	void setModel(const Model* model);
	void setShader(const ShaderProgram* shader);
	void setColor(const glm::vec3& color);

	void setSpecularStrength(const float specularStrength);
	void setSpecularity(const int specularity);

	void render() override;

	void deserialize(const nlohmann::json& jsonObject) override;
	void serialize(nlohmann::json& jsonObject) override;

	std::string name() override { return "ModelRendererComponent"; }
};