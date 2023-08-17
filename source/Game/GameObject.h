#pragma once
#include <string>
#include <vector>
#include <list>
#include <map>

#include <nlohmann/json.hpp>

#include <General/Utils.h>
#include <Game/Component.h>

class GameObject final
{
	friend class ObjectsManager;
	friend class Editor;
public:

	template <typename T>
	T* const AddComponent()
	{
		std::string sComponentName = utils::ClassName<T>(); 
		return static_cast<T*>(mComponents[sComponentName] = new T(this));
	}

	template <typename T>
	T* const GetComponent()
	{
		std::string sComponentName = utils::ClassName<T>(); 
		auto it = mComponents.find(sComponentName);
		return it == mComponents.end() ? nullptr : static_cast<T*>(it->second);
	}

	template <typename T>
	bool Contain() const
	{
		std::string sComponentName = utils::ClassName<T>(); 
		auto it = mComponents.find(sComponentName);
		return it != mComponents.end();
	}

	bool Contain(const std::string& sComponentName) const
	{
		auto it = mComponents.find(sComponentName);
		return it != mComponents.end();
	}

	GameObject* AddGameObject();

	GameObject* GetParent() const;
	const std::string& GetName() const;
	const std::vector<std::string>& GetTags() const;

	void RemoveTag(const std::string sTag);
	void AddTag(const std::string sTag);

	void Serialize(nlohmann::json& jsonObject);
	void Deserialize(const nlohmann::json& jsonObject);

private:
	GameObject(GameObject* pParent);
	GameObject();
	~GameObject();

private:
	GameObject* mpParent;
	std::string msName;
	std::vector<std::string> mTags;
	
	std::map<std::string, Component*> mComponents;
	std::list<GameObject*> mChildren;
};
