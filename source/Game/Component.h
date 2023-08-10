#pragma once
#include <nlohmann/json.hpp>

class GameObject;

class Component 
{
protected:
	GameObject* pGameObject;

public:
	Component(GameObject* pObject) : pGameObject(pObject) {}
	virtual ~Component() {}

	virtual void Deserialize(const nlohmann::json& jsonObject) = 0;
	virtual void Serialize(nlohmann::json& jsonObject) = 0;
	virtual std::string Name() = 0;

	GameObject* const GetGameObject() { return pGameObject; }
};
