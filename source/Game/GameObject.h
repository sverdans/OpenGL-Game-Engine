#pragma once
#include <map>
#include <vector>
#include <string>

#include <nlohmann/json.hpp>

#include "Components/Component.h"

class GameObject final
{
private:
	friend class ObjectsManager;
	friend class Editor;

	std::string name;
	std::vector<std::string> tags;
	GameObject* parent;
	
	std::map<std::string, Component*> components;
	std::vector<GameObject*> children;

	GameObject() : parent(nullptr) {};

	~GameObject() 
	{
		for (auto it = components.begin(); it != components.end(); ++it)
			delete it->second;
		components.clear();

		for (auto& it : children)
			it->parent = nullptr;

		children.clear();
	}

public:

	template <typename T>
	T* const addComponent()
	{
		return static_cast<T*>(components[typeid(T).name()] = new T(this));
	}

	template <typename T>
	T* const getComponent()
	{
		return static_cast<T*>(components[typeid(T).name()]);
	}

	template <typename T>
	bool contain()
	{
		auto it = components.find(typeid(T).name());
		return it != components.end();
	}

	GameObject* addGameObject()
	{
		auto object = new GameObject();
		object->parent = this;
		children.push_back(object);
		return object;
	}

	GameObject* getParent() { return parent; }

	const std::vector<std::string>& getTags() const { return tags; }

	const std::string& getName() const { return name; }

	void addTag(const std::string tag) { tags.push_back(tag); }

	void removeTag(const std::string tag) 
	{
		for (auto it = tags.begin(); it != tags.end(); ++it)
			if (*it == tag)
			{
				tags.erase(it);
				break;
			}
	}

	void serialize(nlohmann::json& jsonObject);

	void deserialize(const nlohmann::json& jsonObject);
};
