#pragma once
#include <string>

#include <Game/Components/RendererComponent.h>

class Model;
class ShaderProgram;

class ModelRendererComponent : public RendererComponent
{
private:
	Renderer::EnDrawMode meDrawMode;

	const Model* mpModel { nullptr };
	const ShaderProgram* mpShader { nullptr };

	bool mlUseMaterial;
	glm::vec3 mColor;

	float mSpecularStrength;
	int mSpecularity;

public:
	ModelRendererComponent() = delete;
	ModelRendererComponent(const ModelRendererComponent&) = delete;
	ModelRendererComponent& operator = (const ModelRendererComponent&) = delete;
	~ModelRendererComponent() = default;

	ModelRendererComponent(GameObject* pGameObject);

	void SetModel(const Model* pModel);
	void SetShader(const ShaderProgram* pShader);
	void SetColor(const glm::vec3& color);

	void SetSpecularStrength(const float specularStrength);
	void SetSpecularity(const int specularity);

	void Render() override;

	void Deserialize(const nlohmann::json& jsonObject) override;
	void Serialize(nlohmann::json& jsonObject) override;

	std::string Name() override { return "ModelRendererComponent"; }
};