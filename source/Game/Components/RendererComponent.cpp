#include <Game/Systems/RendererSystem.h>

#include <Game/GameObject.h>
#include <Game/Components/RendererComponent.h>

RendererComponent::RendererComponent(GameObject* pGameObject) 
	: Component(pGameObject)
	, mlRenderEnable(true)
{
	RendererSystem::AddToRenderQueue(this);
}

RendererComponent::~RendererComponent()
{
	RendererSystem::RemoveFromRenderQueue(this);
}