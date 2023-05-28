#include <Game/Components/LightingComponent.h>
#include <Game/Systems/LightingSystem.h>

LightingComponent::LightingComponent(GameObject* gameObject) : Component(gameObject) 
{
	LightingSystem::addLightingComponent(this);
}


LightingComponent::~LightingComponent()
{
	LightingSystem::removeLightingComponent(this);
}

