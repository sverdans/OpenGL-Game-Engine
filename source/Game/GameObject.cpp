#include <Game/GameObject.h>
#include <Game/ObjectsManager.h>

GameObject::GameObject()
	: mpParent(nullptr)
{ }

GameObject::GameObject(GameObject* pParent)
	: mpParent(pParent)
{ }

GameObject::~GameObject()
{
	for (auto& [sName, pComponent] : mComponents)
		delete pComponent;
	mComponents.clear();

	for (auto& it : mChildren)
		it->mpParent = nullptr;

	mChildren.clear();
}

GameObject* GameObject::AddGameObject()
{
	auto pChild = new GameObject(this);
	mChildren.push_back(pChild);
	return pChild;
}

GameObject* GameObject::GetParent() const { return mpParent; }
const std::string& GameObject::GetName() const { return msName; }
const std::vector<std::string>& GameObject::GetTags() const { return mTags; }

void GameObject::AddTag(const std::string sTag)
{
	mTags.push_back(sTag);
}

void GameObject::RemoveTag(const std::string sTag)
{
	for (auto it = mTags.begin(); it != mTags.end(); ++it)
	{
		if (*it == sTag)
		{
			mTags.erase(it);
			break;
		}
	}
}

void GameObject::Serialize(nlohmann::json& jsonObject)
{
	nlohmann::json jsonTags;
	for (const auto& sTag : mTags)
		jsonTags.push_back(sTag);

	nlohmann::json jsonComponents;
	for (auto& [sName, pComponent] : mComponents)
		jsonComponents[pComponent->Name()] = pComponent->Serialize();

	nlohmann::json jsonChildren;
/*
	if (!mChildren.empty())
	{
		for (const auto& obj : mChildren)
			jsonObject["GameObjects"].push_back(obj->msName);
	}
*/

	jsonObject = {
		{ "name", msName },
		{ "tags", jsonTags },
		{ "components", jsonComponents },
		{ "children", jsonChildren },
	};
}

void GameObject::Deserialize(const nlohmann::json& jsonObject)
{
	msName = jsonObject["name"];

	if (jsonObject.contains("tags"))
	{
		mTags.clear();
		for (const auto& jsonTag : jsonObject["tags"])
			AddTag(jsonTag);
	}

	auto& jsonComponents = jsonObject["components"];
	for (auto& jsonComponent : jsonComponents.items())
	{
		std::cout << "key: " << jsonComponent.key() << ", value:" << jsonComponent.value() << '\n';
	}

/*
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
*/

	if (jsonObject.contains("GameObjects"))
	{
		mChildren.clear();

		for (const auto& sChildrenName : jsonObject["GameObjects"])
		{
			if (auto pChildren = ObjectsManager::Instance().FindByName(sChildrenName))
			{
				mChildren.push_back(pChildren);
				pChildren->mpParent = this;
			}
		}
	}
}