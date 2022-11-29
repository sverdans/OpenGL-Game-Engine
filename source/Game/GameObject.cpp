#define RAPIDJSON_HAS_STDSTRING 1

#include "GameObject.h"

#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"
#include "Components/ModelRendererComponent.h"
#include "Components/LightingComponent.h"

//#include "Components/ShapeRendererComponent.h"
//#include "Components/HyperboloidRendererComponent.h"

void GameObject::serialize(nlohmann::json& jsonObject)
{
	jsonObject["name"] = name;

	for (const auto& tag : tags)
		jsonObject["tags"].push_back(tag);

	for (auto& componentIt : components)
		componentIt.second->serialize(jsonObject);
}

void GameObject::deserialize(const nlohmann::json& jsonObject)
{
	name = jsonObject["name"];

	for (const auto& jsonTag : jsonObject["tags"])
		addTag(jsonTag);

	if (jsonObject.contains("TransformComponent"))
	{
		auto component = contain<TransformComponent>() ? getComponent<TransformComponent>() : addComponent<TransformComponent>(); 
		component->deserialize(jsonObject[component->name()]);
	}

	if (jsonObject.contains("CameraComponent"))
	{
		auto component = contain<CameraComponent>() ? getComponent<CameraComponent>() : addComponent<CameraComponent>();
		component->deserialize(jsonObject[component->name()]);
	}

	if (jsonObject.contains("ModelRendererComponent"))
	{
		auto component = contain<ModelRendererComponent>() ? getComponent<ModelRendererComponent>() : addComponent<ModelRendererComponent>();
		component->deserialize(jsonObject[component->name()]);
	}

	if (jsonObject.contains("LightingComponent"))
	{
		auto component = contain<LightingComponent>() ? getComponent<LightingComponent>() : addComponent<LightingComponent>();
		component->deserialize(jsonObject[component->name()]);
	}

	/*auto childrenIt = jsonObject.find("GameObjects");

	if (childrenIt != jsonObject.MemberEnd())
	{
		const auto childrenArray = childrenIt->value.GetArray();
		for (const auto& jsonChildrenObject : childrenArray)
		{
			auto childrenObject = addGameObject();
			childrenObject->deserialize(jsonChildrenObject);
		}
	}*/
}