#pragma once
#include <Game/Components/BehaviourComponent.h>

class TransformComponent;

class ClockComponent : public BehaviourComponent
{
public:
	ClockComponent(GameObject* pGameObject) : BehaviourComponent(pGameObject) {}
	ClockComponent(const ClockComponent&) = delete;
	ClockComponent& operator = (const ClockComponent&) = delete;
	~ClockComponent() = default;

public:
	void Init() override;
	void Update() override;

	Component* Clone() override;
	std::string Name() override;
	json Serialize() override;
	void Deserialize(const json& jsonObject) override;

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
};
