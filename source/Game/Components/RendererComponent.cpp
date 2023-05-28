#include <Game/Systems/RendererSystem.h>

#include <Game/GameObject.h>
#include <Game/Components/RendererComponent.h>

RendererComponent::RendererComponent(GameObject* gameObject) : Component(gameObject)
{
	RendererSystem::addToRenderQueue(this);
}

RendererComponent::~RendererComponent()
{
	RendererSystem::removeFromRenderQueue(this);
}