#pragma once
#include <vector>

#include <glm/vec3.hpp>
#include "../../System/InputHandler.h"
#include "../../System/Time.h"
#include "../GameObject.h"

#include "BehaviourComponent.h"

#include "ModelRendererComponent.h"
#include "LightingComponent.h"

class L4T3_Component : public BehaviourComponent
{
private:
	unsigned int type;	// 0 - yellow | 1 - purple
	double lastUpdate = 0;

public:

	L4T3_Component(GameObject* gameObject) : BehaviourComponent(gameObject) {}
	L4T3_Component(const L4T3_Component&) = delete;
	L4T3_Component& operator = (const L4T1_Component&) = delete;
	~L4T3_Component() = default;

	void init() override
	{
		auto MRC = gameObject->getComponent<ModelRendererComponent>();
		auto LC = gameObject->getComponent<LightingComponent>();

		LC->setIntensity(0.5f);

		if (type == 0)
		{
			MRC->setColor(glm::vec3(255.f / 255.f, 215.f / 255.f, 0.f));
			LC->setColor(glm::vec3(255.f / 255.f, 215.f / 255.f, 0.f));
		}
		else
		{
			MRC->setColor(glm::vec3(154.f / 255.f, 50.f / 255.f, 205.f / 255.f));
			LC->setColor(glm::vec3(154.f / 255.f, 50.f / 255.f, 205.f / 255.f));
		}
	}

	void update() override
	{
		if (Time::getTime() - lastUpdate < 500)
			return;

		auto& keys = InputHandler::getKeys();

		if ((keys[GLFW_KEY_P] && type == 1) || (keys[GLFW_KEY_Y] && type == 0))
		{
			auto LC = gameObject->getComponent<LightingComponent>();
			float intensity = LC->getIntensity() + 0.05f;
			
			if (intensity > 1)
				LC->setIntensity(0.f);
			else
				LC->setIntensity(intensity);

			lastUpdate = Time::getTime();
		}
	}

	void deserialize(const nlohmann::json& jsonObject) override 
	{
		type = jsonObject["type"];
	}

	void serialize(nlohmann::json& jsonObject) override
	{
		nlohmann::json component;
		component["type"] = type;
		jsonObject[name()] = component;
	}

	std::string name() override { return "L4T3_Component"; }
};