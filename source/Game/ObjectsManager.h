#pragma once
#include <map>
#include <list>
#include <string>

#include <General/Utils.h>
#include <Game/Component.h>
#include <Game/GameObject.h>
#include <spdlog/spdlog.h>

#define DECLARE_COMPONENT(NAME) \
void* p ## NAME = ObjectsManager::Instance().DeclareComponent<NAME>();

class ObjectsManager final
{
	friend class Editor;
public:
	static ObjectsManager& Instance() 
	{ 
		static ObjectsManager instance;
		return instance;
	}

	template<СomponentInherited T>
	Component* DeclareComponent()
	{
		std::string sName = utils::ClassName<T>();
		auto pComponent = new T(nullptr);
		mComponents.emplace(sName, pComponent);
		return pComponent;
	}

	void Clear();
	bool Load(const std::string& sFilePath);

	GameObject* FindByTag(const std::string sName);
	GameObject* FindByName(const std::string sName);
	Component* GetComponentInstance(const std::string& sName);

private:
	ObjectsManager();
	~ObjectsManager() = default;
	ObjectsManager(const ObjectsManager&) = delete;
	ObjectsManager(const ObjectsManager&&) = delete;
	ObjectsManager& operator = (const ObjectsManager&) = delete;
	ObjectsManager& operator = (const ObjectsManager&&) = delete;

private:
	std::list<GameObject*> mGameObjects;
	std::map<std::string, Component*> mComponents;
};
