#pragma once
#include <glm/vec2.hpp>
#include <functional>

#include "Component.h"
#include "../../System/Time.h"
#include "../../Resources/ResourceManager.h"

class RendererComponent : public Component
{
public:
	bool renderEnable = true;

	RendererComponent() = delete;
	RendererComponent(const RendererComponent&) = delete;
	RendererComponent& operator = (const RendererComponent&) = delete;

	RendererComponent(GameObject* gameObject);
	~RendererComponent();

	virtual void render() = 0;
	virtual void update() {};

	void deserialize(const nlohmann::json& jsonObject) override {}
	void serialize(nlohmann::json& jsonObject) override {}
};