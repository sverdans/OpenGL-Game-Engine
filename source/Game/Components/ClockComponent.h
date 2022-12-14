#pragma once
#include "BehaviourComponent.h"

class TransformComponent;

class ClockComponent : public BehaviourComponent
{
private:
	TransformComponent* secondTransform;
	TransformComponent* minuteTransform;
	TransformComponent* hourTransform;

public:

	ClockComponent(GameObject* gameObject) : BehaviourComponent(gameObject) {}
	ClockComponent(const ClockComponent&) = delete;
	ClockComponent& operator = (const ClockComponent&) = delete;
	~ClockComponent() = default;

	void init() override;

	void update() override;
	void deserialize(const nlohmann::json& jsonObject) override;

	void serialize(nlohmann::json& jsonObject) override;

	std::string name() override;
};