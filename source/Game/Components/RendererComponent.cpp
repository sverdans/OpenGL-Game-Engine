#include <Game/Systems/RendererSystem.h>

#include <Game/GameObject.h>
#include <Game/Components/RendererComponent.h>

RendererComponent::RendererComponent(GameObject* pGameObject) 
	: Component(pGameObject)
	, mlRenderEnable(true)
{
	if (pGameObject)
		RendererSystem::Instance().AddToRenderQueue(this);
}

RendererComponent::~RendererComponent()
{
	RendererSystem::Instance().RemoveFromRenderQueue(this);
}