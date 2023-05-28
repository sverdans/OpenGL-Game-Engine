#include <Game/Components/BehaviourComponent.h>
#include <Game/Systems/BehaviourSystem.h>

BehaviourComponent::BehaviourComponent(GameObject* gameObject) : Component(gameObject)
{
	BehaviourSystem::addToUpdateQueue(this);
}

BehaviourComponent::~BehaviourComponent()
{
	BehaviourSystem::removeFromUpdateQueue(this);
}
