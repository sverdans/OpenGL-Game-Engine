#include <Game/Components/LightingComponent.h>
#include <Game/Systems/LightingSystem.h>

DECLARE_COMPONENT(LightingComponent)

LightingComponent::LightingComponent(GameObject* pGameObject) : Component(pGameObject) 
{
	LightingSystem::Instance().AddLightingComponent(this);
}

LightingComponent::~LightingComponent()
{
	LightingSystem::Instance().RemoveLightingComponent(this);
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

Component* LightingComponent::Clone() { return new LightingComponent(nullptr); }
std::string LightingComponent::Name() { return "LightingComponent"; }

json LightingComponent::Serialize()
{
	json colorJson {
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

void LightingComponent::Deserialize(const json& jsonObject)
{
	int r = jsonObject["color"]["r"];
	int g = jsonObject["color"]["g"];
	int b = jsonObject["color"]["b"];
	mColor = { r, g, b };

	SetIntensity(jsonObject["intensity"]);
	meType = jsonObject["type"];
}
