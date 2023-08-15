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

	virtual Component* Clone() = 0;
	virtual std::string Name() = 0;
	virtual nlohmann::json Serialize() = 0;
	virtual void Deserialize(const nlohmann::json& jsonObject) = 0;

	GameObject* const GetGameObject() const { return mpGameObject; }
	void SetGameObject(GameObject* pGameObject) { mpGameObject = pGameObject; }
};
