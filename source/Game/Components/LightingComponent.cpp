#include "LightingComponent.h"
#include "../Systems/LightingSystem.h"


LightingComponent::LightingComponent(GameObject* gameObject) : Component(gameObject) 
{
	LightingSystem::addLightingComponent(this);
}


LightingComponent::~LightingComponent()
{
	LightingSystem::removeLightingComponent(this);
}

