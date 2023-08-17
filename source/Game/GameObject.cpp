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

	mTags.clear();
	for (const auto& jsonTag : jsonObject["tags"])
		AddTag(jsonTag);

	const auto& jsonComponents = jsonObject["components"];
	for (auto& it : jsonComponents.items())
	{
		const std::string sComponentName = it.key();
		const nlohmann::json& jsonComponent = it.value();

		std::cout << "key: " << sComponentName << ", value:" << jsonComponent << std::endl;
		
		if (Contain(sComponentName))
		{
			std::cout << "Deserialize " << sComponentName << std::endl;
			mComponents[sComponentName]->Deserialize(jsonComponent);
		}
		else
		{
			auto pInstace = ObjectsManager::Instance().GetComponentInstance(sComponentName);
			auto pComponent = pInstace->Clone();
			pComponent->SetGameObject(this);
			pComponent->Deserialize(jsonComponent);
			mComponents[sComponentName] = pComponent;
		}
	}

	const auto& jsonChildren = jsonObject["children"];
	/*
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
	*/
}