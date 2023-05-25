#pragma once
#include <vector>

#include <glm/vec3.hpp>

#include "../../System/InputHandler.h"
#include "../../System/Time.h"
#include "../GameObject.h"

#include "BehaviourComponent.h"

#include "TransformComponent.h"
#include "ModelRendererComponent.h"
#include "LightingComponent.h"

class L4T4_Component : public BehaviourComponent
{
private:
	float radius = 3.f;
	float velocity = 0.1f;
	float delta = 0.004f;
	float a = 0;

	GameObject* focusGameObject;
	
public:

	L4T4_Component(GameObject* gameObject) : BehaviourComponent(gameObject) {}
	L4T4_Component(const L4T4_Component&) = delete;
	L4T4_Component& operator = (const L4T4_Component&) = delete;
	~L4T4_Component() = default;

	void init() override;

	void update() override;

	void deserialize(const nlohmann::json& jsonObject) override 
	{
		radius = jsonObject["radius"];
	}

	void serialize(nlohmann::json& jsonObject) override
	{
		nlohmann::json component;
		component["radius"] = radius;
		jsonObject[name()] = component;
	}

	std::string name() override { return "L4T4_Component"; }
};