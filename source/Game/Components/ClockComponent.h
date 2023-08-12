#pragma once
#include <Game/Components/BehaviourComponent.h>

class TransformComponent;

class ClockComponent : public BehaviourComponent
{
private:
	TransformComponent* secondTransform;
	TransformComponent* minuteTransform;
	TransformComponent* hourTransform;

	unsigned int hourDirection = -1;
	unsigned int minuteDirection = -1;
	unsigned int secondDirection = -1;

	unsigned int hour;
	unsigned int minute;
	unsigned int second;

	float scale = 1;

public:

	ClockComponent(GameObject* gameObject) : BehaviourComponent(gameObject) {}
	ClockComponent(const ClockComponent&) = delete;
	ClockComponent& operator = (const ClockComponent&) = delete;
	~ClockComponent() = default;

	void Init() override;
	void Update() override;

	void Deserialize(const nlohmann::json& jsonObject) override;
	void Serialize(nlohmann::json& jsonObject) override;

	std::string Name() override;
};
