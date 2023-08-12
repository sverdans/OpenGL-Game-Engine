#include <Game/Systems/BehaviourSystem.h>
#include <Game/Components/BehaviourComponent.h>

void BehaviourSystem::addToUpdateQueue(BehaviourComponent* BC)
{
	updateQueue.push_back(BC);
}

void BehaviourSystem::removeFromUpdateQueue(BehaviourComponent* BC)
{
	updateQueue.remove(BC);
}

void BehaviourSystem::init()
{
	for (auto it : updateQueue)
		it->Init();
}

void BehaviourSystem::update()
{
	for (auto it : updateQueue)
		it->Update();
}

void BehaviourSystem::lateUpdate()
{
	for (auto it : updateQueue)
		it->LateUpdate();
}

void BehaviourSystem::onPreRender()
{
	for (auto it : updateQueue)
		it->OnPreRender();
}


std::list<BehaviourComponent*>& BehaviourSystem::getUpdateQueue()
{
	return updateQueue;
}

std::list<BehaviourComponent*> BehaviourSystem::updateQueue;
