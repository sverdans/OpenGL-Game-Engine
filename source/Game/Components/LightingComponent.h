#pragma once
#include <glm/vec3.hpp>

#include <Game/Component.h>

class LightingComponent : public Component
{
	friend class LightingSystem;
public:
	enum Type
	{
		Directional,
		Point
	};

public:
	
	float GetIntensity();
	float SetIntensity(float intensity);
	void SetColor(const glm::vec3& color);
	
	std::string Name() override;
	nlohmann::json Serialize() override;
	void Deserialize(const nlohmann::json& jsonObject) override;

public:
	LightingComponent(GameObject* gameObject);
	LightingComponent() = delete;
	~LightingComponent();
	LightingComponent(const LightingComponent&) = delete;
	LightingComponent& operator = (const LightingComponent&) = delete;

private:
	Type meType;
	glm::vec3 mColor;
	float mIntensity = 1;
};
