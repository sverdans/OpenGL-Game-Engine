#include <Game/Components/BehaviourComponent.h>
#include <Game/Systems/BehaviourSystem.h>

BehaviourComponent::BehaviourComponent(GameObject* pGameObject) : Component(pGameObject)
{
    if (pGameObject)
	    BehaviourSystem::Instance().AddToUpdateQueue(this);
}

BehaviourComponent::~BehaviourComponent()
{
	BehaviourSystem::Instance().RemoveFromUpdateQueue(this);
}
