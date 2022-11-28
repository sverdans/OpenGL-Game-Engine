#pragma once
#include "RendererComponent.h"

#include "../GameObject.h"
#include "../Systems/RendererSystem.h"

RendererComponent::RendererComponent(GameObject* gameObject) : Component(gameObject)
{
	RendererSystem::addToRenderQueue(this);
}

RendererComponent::~RendererComponent()
{
	RendererSystem::removeFromRenderQueue(this);
}