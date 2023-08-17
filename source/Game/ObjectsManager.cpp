#include <typeinfo>
#include <algorithm>

#include <Game/Component.h>
#include <Game/GameObject.h>
#include <Game/ObjectsManager.h>

using json = nlohmann::ordered_json;

ObjectsManager::ObjectsManager() { }

Component* ObjectsManager::GetComponentInstance(const std::string& sName)
{
	auto it = mComponents.find(sName);
	if (it == mComponents.end())
		return nullptr;

	return it->second;
}

void ObjectsManager::Clear()
{
	for (auto it : mGameObjects)
		delete it;

	mGameObjects.clear();
}

bool ObjectsManager::Load(const std::string& sFilePath)
{
	json sourceObject;

	if (!utils::ParseJsonFile(sFilePath, sourceObject))
	{
		std::cout << "parse error" << std::endl;
		// warning
		return false;
	}

	for (const auto& jsonGameObject : sourceObject["objects"])
	{
		GameObject* object = new GameObject();
		object->Deserialize(jsonGameObject);
		mGameObjects.push_back(object);
	}
	return true;
}

GameObject* ObjectsManager::FindByTag(const std::string sTag)
{
	for (auto object : mGameObjects)
	{
		auto tags = object->GetTags();
		auto it = std::find(tags.begin(), tags.end(), sTag);
		if (it != tags.end())
			return object;
	}
	return nullptr;
}

GameObject* ObjectsManager::FindByName(const std::string sName)
{
	for (auto object : mGameObjects)
	{
		if (object->msName == sName)
			return object;
	}
	return nullptr;
}
