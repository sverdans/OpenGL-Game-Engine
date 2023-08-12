#pragma once
#include <string>
#include <nlohmann/json.hpp>

class GameObject;

class Component 
{
private:
	GameObject* mpGameObject;

public:
	Component(GameObject* pGameObject) 
		: mpGameObject(pGameObject) { }
	virtual ~Component() { }

	virtual void Deserialize(const nlohmann::json& jsonObject) = 0;
	virtual void Serialize(nlohmann::json& jsonObject) = 0;
	virtual std::string Name() = 0;

	GameObject* const GetGameObject() const { return mpGameObject; }
};
