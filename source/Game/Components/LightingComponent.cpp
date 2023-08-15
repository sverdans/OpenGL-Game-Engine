#include <Game/Components/LightingComponent.h>
#include <Game/Systems/LightingSystem.h>

LightingComponent::LightingComponent(GameObject* gameObject) : Component(gameObject) 
{
	LightingSystem::addLightingComponent(this);
}


LightingComponent::~LightingComponent()
{
	LightingSystem::removeLightingComponent(this);
}

float LightingComponent::GetIntensity() 
{ 
	return mIntensity; 
}

float LightingComponent::SetIntensity(float intensity) 
{
	return mIntensity = mIntensity > 0.f 
		? (mIntensity < 1.f ? mIntensity : 1.f) 
		: 0.f;
}

void LightingComponent::SetColor(const glm::vec3& color) 
{ 
	mColor = color;
}

std::string LightingComponent::Name() { return "LightingComponent"; }

void LightingComponent::Deserialize(const nlohmann::json& jsonObject)
{
	int r = jsonObject["color"]["r"];
	int g = jsonObject["color"]["g"];
	int b = jsonObject["color"]["b"];
	mColor = { r, g, b };

	SetIntensity(jsonObject["intensity"]);
	meType = jsonObject["type"];
}

nlohmann::json LightingComponent::Serialize()
{
	nlohmann::json colorJson {
		{ "r", mColor.r },
		{ "g", mColor.g },
		{ "b", mColor.b },
	};

	return {
		{ "type", meType },
		{ "intensity", mIntensity },
		{ "color", colorJson },
	};
}
