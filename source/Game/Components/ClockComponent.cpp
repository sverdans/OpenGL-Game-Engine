#include "ClockComponent.h"
#include "TransformComponent.h"
#include "../ObjectsManager.h"
#include "../GameObject.h"


void ClockComponent::init() 
{
	secondTransform = ObjectsManager::findByName("ArrowSecond")->getComponent<TransformComponent>();
	minuteTransform = ObjectsManager::findByName("ArrowMinute")->getComponent<TransformComponent>();
	hourTransform = ObjectsManager::findByName("ArrowHour")->getComponent<TransformComponent>();
}

void ClockComponent::update() 
{
	auto secondRotation = secondTransform->getRotation();
	secondRotation.z -= Time::getDeltaTime() * 0.005;
	secondTransform->setRotation(secondRotation);
}

void ClockComponent::deserialize(const nlohmann::json& jsonObject)
{
}

void ClockComponent::serialize(nlohmann::json& jsonObject)
{
	nlohmann::json component;
	jsonObject[name()] = component;
}


std::string ClockComponent::name() { return "ClockComponent"; }
