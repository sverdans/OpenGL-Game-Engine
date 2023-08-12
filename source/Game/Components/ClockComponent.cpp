#include <Game/GameObject.h>
#include <Game/ObjectsManager.h>

#include <Game/Components/ClockComponent.h>
#include <Game/Components/TransformComponent.h>

void ClockComponent::Init() 
{
	auto& manager = ObjectsManager::Instance(); 
	
	secondTransform = manager.FindByName("ArrowSecond")->GetComponent<TransformComponent>();
	minuteTransform = manager.FindByName("ArrowMinute")->GetComponent<TransformComponent>();
	hourTransform   = manager.FindByName("ArrowHour")->GetComponent<TransformComponent>();
}

void ClockComponent::Update() 
{
	auto hourRotation = hourTransform->GetRotation();
	auto minuteRotation = minuteTransform->GetRotation();
	auto secondRotation = secondTransform->GetRotation();
	
	hourRotation.z   -= Time::getDeltaTime() * scale * 0.005 / 60 / 12;
	minuteRotation.z -= Time::getDeltaTime() * scale * 0.005 / 60;
	secondRotation.z -= Time::getDeltaTime() * scale * 0.005;

	hourTransform->SetRotation(hourRotation);
	minuteTransform->SetRotation(minuteRotation);
	secondTransform->SetRotation(secondRotation);
}

void ClockComponent::Deserialize(const nlohmann::json& jsonObject)
{
	scale = jsonObject["scale"];
}

void ClockComponent::Serialize(nlohmann::json& jsonObject)
{
	jsonObject[Name()] = {
		{"scale", scale}
	};
}

std::string ClockComponent::Name() { return "ClockComponent"; }
