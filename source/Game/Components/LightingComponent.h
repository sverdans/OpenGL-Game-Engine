#pragma once
#include <glm/vec3.hpp>

#include "Component.h"

class LightingComponent : public Component
{
private:
	friend class LightingSystem;

	glm::vec3 color;
	float specularStrength = 0.5f;

public:
	LightingComponent(GameObject* gameObject);
	LightingComponent() = delete;
	LightingComponent(const LightingComponent&) = delete;
	LightingComponent& operator = (const LightingComponent&) = delete;
	~LightingComponent();

	std::string name() override { return "LightingComponent"; }

	void deserialize(const nlohmann::json& jsonObject) override
	{
		color = glm::vec3(jsonObject["color"]["r"],
						  jsonObject["color"]["g"],
						  jsonObject["color"]["b"]);
	}

	void serialize(nlohmann::json& jsonObject) override
	{
		nlohmann::json colorJson;
		colorJson["r"] = color.r;
		colorJson["g"] = color.g;
		colorJson["b"] = color.b;
	
		nlohmann::json component;
		component["color"] = colorJson;

		jsonObject[name()] = component;
	}
};
