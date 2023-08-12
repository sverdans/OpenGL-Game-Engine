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

private:

	Type meType;
	glm::vec3 mColor;
	float mIntensity = 1;

public:
	LightingComponent(GameObject* gameObject);
	LightingComponent() = delete;
	LightingComponent(const LightingComponent&) = delete;
	LightingComponent& operator = (const LightingComponent&) = delete;
	~LightingComponent();

	std::string Name() override { return "LightingComponent"; }
	
	float GetIntensity() 
	{ 
		return mIntensity; 
	}

	float SetIntensity(float intensity) 
	{
		return mIntensity = mIntensity > 0.f 
			? (mIntensity < 1.f ? mIntensity : 1.f) 
			: 0.f;
	}
	
	void SetColor(const glm::vec3& color) 
	{ 
		mColor = color;
	}
	
	void Deserialize(const nlohmann::json& jsonObject) override
	{
		int r = jsonObject["color"]["r"];
		int g = jsonObject["color"]["g"];
		int b = jsonObject["color"]["b"];
		mColor = { r, g, b };

		SetIntensity(jsonObject["intensity"]);
		meType = jsonObject["type"];
	}

	void Serialize(nlohmann::json& jsonObject) override
	{
		nlohmann::json colorJson {
			{ "r", mColor.r },
			{ "g", mColor.g },
			{ "b", mColor.b },
		};

		nlohmann::json component {
			{ "type", meType },
			{ "intensity", mIntensity },
			{ "color", colorJson },
		};

		jsonObject[Name()] = component;
	}
};
