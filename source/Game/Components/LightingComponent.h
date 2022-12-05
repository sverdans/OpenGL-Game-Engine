#pragma once
#include <glm/vec3.hpp>

#include "Component.h"

class LightingComponent : public Component
{
public:
	enum class Type
	{
		Directional,
		Point
	};

private:
	friend class LightingSystem;

	Type type;
	glm::vec3 color;
	float intensity = 1;

public:
	LightingComponent(GameObject* gameObject);
	LightingComponent() = delete;
	LightingComponent(const LightingComponent&) = delete;
	LightingComponent& operator = (const LightingComponent&) = delete;
	~LightingComponent();

	std::string name() override { return "LightingComponent"; }
	
	float getIntensity() { return intensity; }
	void setIntensity(float intensity) { this->intensity = intensity > 0.f ? (intensity < 1.f ? intensity : 1.f) : 0.f; }
	
	void setColor(const glm::vec3& color) { this->color = color; }
	
	void deserialize(const nlohmann::json& jsonObject) override
	{
		color = glm::vec3(jsonObject["color"]["r"],
						  jsonObject["color"]["g"],
						  jsonObject["color"]["b"]);

		setIntensity(jsonObject["intensity"]);
		type = static_cast<Type>(jsonObject["type"]);
	}

	void serialize(nlohmann::json& jsonObject) override
	{
		nlohmann::json colorJson;
		colorJson["r"] = color.r;
		colorJson["g"] = color.g;
		colorJson["b"] = color.b;

		nlohmann::json component;
		component["type"] = static_cast<int>(type);
		component["color"] = colorJson;
		component["intensity"] = intensity;

		jsonObject[name()] = component;
	}
};
