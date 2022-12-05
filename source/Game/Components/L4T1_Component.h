#pragma once
#include <vector>

#include <glm/vec3.hpp>
#include "../../System/InputHandler.h"
#include "../../System/Time.h"
#include "../GameObject.h"

#include "BehaviourComponent.h"

#include "ModelRendererComponent.h"
#include "LightingComponent.h"

class L4T1_Component : public BehaviourComponent
{
private:
	std::vector<glm::vec3> colors;
	unsigned int currentColor = 0;
	double lastUpdate = 0;

public:

	L4T1_Component(GameObject* gameObject) : BehaviourComponent(gameObject) {}
	L4T1_Component(const L4T1_Component&) = delete;
	L4T1_Component& operator = (const L4T1_Component&) = delete;
	~L4T1_Component() = default;

	void init() override
	{
		colors = {
			glm::vec3(210.f / 255.f,		  0.f,			  0.f), // red
			glm::vec3(255.f / 255.f, 127.f / 255.f,			  0.f), // orange
			glm::vec3(255.f / 255.f, 215.f / 255.f,			  0.f), // yellow
			glm::vec3(127.f / 255.f, 255.f / 255.f,			  0.f), // green
			glm::vec3(			0.f, 191.f / 255.f, 255.f / 255.f), // blue
			glm::vec3( 17.f / 255.f,  17.f / 255.f, 205.f / 255.f), // dark blue
			glm::vec3(154.f / 255.f,  50.f / 255.f, 205.f / 255.f)  // purple
		};

		auto MRC = gameObject->getComponent<ModelRendererComponent>();
		if (MRC) MRC->setColor(colors[currentColor]);

		auto LC = gameObject->getComponent<LightingComponent>();
		if (LC) LC->setColor(colors[currentColor]);
	}

	void update() override
	{
		if (Time::getTime() - lastUpdate < 500)
			return;

		auto& keys = InputHandler::getKeys();

		if (keys[GLFW_KEY_EQUAL])
		{
			currentColor++;
			currentColor %= colors.size();

			lastUpdate = Time::getTime();
		}

		auto MRC = gameObject->getComponent<ModelRendererComponent>();
		if (MRC) MRC->setColor(colors[currentColor]);

		auto LC = gameObject->getComponent<LightingComponent>();
		if (LC) LC->setColor(colors[currentColor]);
	}

	void deserialize(const nlohmann::json& jsonObject) override { }

	void serialize(nlohmann::json& jsonObject) override
	{
		nlohmann::json component;
		jsonObject[name()] = component;
	}

	std::string name() override { return "L4T1_Component"; }
};