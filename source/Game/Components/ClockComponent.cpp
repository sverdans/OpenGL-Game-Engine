#include <Game/GameObject.h>
#include <Game/ObjectsManager.h>

#include <Game/Components/ClockComponent.h>
#include <Game/Components/TransformComponent.h>

void ClockComponent::init() 
{
	secondTransform = ObjectsManager::findByName("ArrowSecond")->getComponent<TransformComponent>();
	minuteTransform = ObjectsManager::findByName("ArrowMinute")->getComponent<TransformComponent>();
	hourTransform = ObjectsManager::findByName("ArrowHour")->getComponent<TransformComponent>();
}

void ClockComponent::update() 
{
	auto hourRotation = hourTransform->getRotation();
	auto minuteRotation = minuteTransform->getRotation();
	auto secondRotation = secondTransform->getRotation();
	
	hourRotation.z   -= Time::getDeltaTime() * scale * 0.005 / 60 / 12;
	minuteRotation.z -= Time::getDeltaTime() * scale * 0.005 / 60;
	secondRotation.z -= Time::getDeltaTime() * scale * 0.005;

	hourTransform->setRotation(hourRotation);
	minuteTransform->setRotation(minuteRotation);
	secondTransform->setRotation(secondRotation);
}

void ClockComponent::deserialize(const nlohmann::json& jsonObject)
{
	scale = jsonObject["scale"];
}

void ClockComponent::serialize(nlohmann::json& jsonObject)
{
	nlohmann::json component;
	component["scale"] = scale;
	jsonObject[name()] = component;
}


std::string ClockComponent::name() { return "ClockComponent"; }
