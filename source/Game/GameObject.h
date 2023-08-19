#pragma once
#include <string>
#include <vector>
#include <list>
#include <map>

#include <nlohmann/json.hpp>

#include <General/Utils.h>
#include <Game/Component.h>

using json = nlohmann::ordered_json;

class GameObject final
{
	friend class Editor;
	friend class ObjectsManager;
public:

	template<СomponentInherited T>
	T* const AddComponent()
	{
		std::string sComponentName = utils::ClassName<T>();
		if (mComponents.find(sComponentName) == mComponents.end())
		{
			return nullptr;
		}

		Component* pComponent = new T(this);
		mComponents[sComponentName] = pComponent;
		return static_cast<T*>(pComponent);
	}

	template<СomponentInherited T>
	T* const GetComponent()
	{
		std::string sComponentName = utils::ClassName<T>(); 
		auto it = mComponents.find(sComponentName);

		return it != mComponents.end() 
			? static_cast<T*>(it->second) 
			: nullptr;
	}

	template<СomponentInherited T>
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

	json Serialize();
	void Deserialize(const json& jsonObject);

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
