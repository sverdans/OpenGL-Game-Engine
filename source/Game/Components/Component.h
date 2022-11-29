#pragma once
#include <nlohmann/json.hpp>

class GameObject;

class Component 
{
protected:
	GameObject* gameObject;
public:
	Component(GameObject* object) : gameObject(object) {}
	virtual ~Component() {}

	GameObject* const getGameObject() { return gameObject; }

	virtual void deserialize(const nlohmann::json& jsonObject) = 0;
	virtual void serialize(nlohmann::json& jsonObject) = 0;

	virtual std::string name() = 0;
};