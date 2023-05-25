#include "ObjectsManager.h"

#include <typeinfo>
#include <algorithm>

#include "GameObject.h"

void ObjectsManager::deleteGameObjects()
{
	for (auto it = gameObjects.begin(); it != gameObjects.end(); ++it)
		delete *it;
	gameObjects.clear();
}

void ObjectsManager::loadGameObjects(const std::string& filePath)
{
	nlohmann::json sourceObject;
	Parser::parseJsonFile(filePath, sourceObject);

	for (const auto& jsonGameObject : sourceObject["GameObjects"])
	{
		GameObject* object = new GameObject();
		object->deserialize(jsonGameObject);
		gameObjects.push_back(object);
	}

}

GameObject* ObjectsManager::findByTag(const std::string tag)
{
	for (auto object : gameObjects)
	{
		auto tags = object->getTags();
		auto it = std::find(tags.begin(), tags.end(), tag);
		if (it != tags.end())
			return object;
	}
	return nullptr;
}

GameObject* ObjectsManager::findByName(const std::string name)
{
	for (auto object : gameObjects)
	{
		if (object->name == name)
			return object;
	}
	return nullptr;
}

std::vector<GameObject*> ObjectsManager::gameObjects;