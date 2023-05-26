#include <Game/GameObject.h>
#include <Game/ObjectsManager.h>

#include <Game/Components/TransformComponent.h>
#include <Game/Components/CameraComponent.h>
#include <Game/Components/ModelRendererComponent.h>
#include <Game/Components/LightingComponent.h>
#include <Game/Components/ClockComponent.h>

#include <Game/Components/L4T1_Component.h>
#include <Game/Components/L4T3_Component.h>
#include <Game/Components/L4T4_Component.h>


void GameObject::serialize(nlohmann::json& jsonObject)
{
	jsonObject["name"] = name;

	if (tags.size() != 0)
		for (const auto& tag : tags)
			jsonObject["tags"].push_back(tag);

	if (children.size() != 0)
		for (const auto& obj : children)
			jsonObject["GameObjects"].push_back(obj->name);

	for (auto& componentIt : components)
		componentIt.second->serialize(jsonObject);
}

void GameObject::deserialize(const nlohmann::json& jsonObject)
{
	name = jsonObject["name"];

	if (jsonObject.contains("tags"))
	{
		tags.clear();
		for (const auto& jsonTag : jsonObject["tags"])
			addTag(jsonTag);
	}

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

	if (jsonObject.contains("LightingComponent"))
	{
		auto component = contain<LightingComponent>() ? getComponent<LightingComponent>() : addComponent<LightingComponent>();
		component->deserialize(jsonObject[component->name()]);
	}

	if (jsonObject.contains("ClockComponent"))
	{
		auto component = contain<ClockComponent>() ? getComponent<ClockComponent>() : addComponent<ClockComponent>();
		component->deserialize(jsonObject[component->name()]);
	}


	if (jsonObject.contains("L4T1_Component"))
	{
		auto component = contain<L4T1_Component>() ? getComponent<L4T1_Component>() : addComponent<L4T1_Component>();
		component->deserialize(jsonObject[component->name()]);
	}

	if (jsonObject.contains("L4T3_Component"))
	{
		auto component = contain<L4T3_Component>() ? getComponent<L4T3_Component>() : addComponent<L4T3_Component>();
		component->deserialize(jsonObject[component->name()]);
	}

	if (jsonObject.contains("L4T4_Component"))
	{
		auto component = contain<L4T4_Component>() ? getComponent<L4T4_Component>() : addComponent<L4T4_Component>();
		component->deserialize(jsonObject[component->name()]);
	}


	if (jsonObject.contains("GameObjects"))
	{
		children.clear();
		for (const auto& childrenName : jsonObject["GameObjects"])
		{
			auto obj = ObjectsManager::findByName(childrenName);
			if (obj)
			{
				children.push_back(obj);
				obj->parent = this;
			}
		}
	}
}