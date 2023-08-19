#pragma once
#include <string>
#include <concepts>
#include <nlohmann/json.hpp>

using json = nlohmann::ordered_json;

class GameObject;
class Component;

template<class T> concept СomponentInherited = std::derived_from<T, Component>;

class Component
{
private:
	GameObject* mpGameObject { nullptr };

public:
	Component(GameObject* pGameObject) 
		: mpGameObject(pGameObject) { }
	virtual ~Component() { }

	virtual Component* Clone() = 0;
	virtual std::string Name() = 0;
	virtual json Serialize() = 0;
	virtual void Deserialize(const json& jsonObject) = 0;

	GameObject* const GetGameObject() const
	{
		return mpGameObject;
	}
	
	void SetGameObject(GameObject* pGameObject)
	{
		mpGameObject = pGameObject;
	}
};
