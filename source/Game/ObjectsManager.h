#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>

#include "Components/RendererComponent.h"
#include "Components/BehaviourComponent.h"

#include "../System/Parser.h"

class GameObject;

class ObjectsManager final
{
private:
	friend class Editor;

	static std::vector<GameObject*> gameObjects;

public:
	ObjectsManager() = delete;
	~ObjectsManager() = delete;
	ObjectsManager(const ObjectsManager&) = delete;
	ObjectsManager& operator = (const ObjectsManager&) = delete;

	static void loadGameObjects(const std::string& filePath);
	static void deleteGameObjects();

	static GameObject* findByTag(const std::string name);
	static GameObject* findByName(const std::string name);
};
