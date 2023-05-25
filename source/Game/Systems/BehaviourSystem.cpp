#include "BehaviourSystem.h"
#include "../Components/BehaviourComponent.h"


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
		it->init();
}

void BehaviourSystem::update()
{
	for (auto it : updateQueue)
		it->update();
}

void BehaviourSystem::lateUpdate()
{
	for (auto it : updateQueue)
		it->lateUpdate();
}

void BehaviourSystem::onPreRender()
{
	for (auto it : updateQueue)
		it->onPreRender();
}


std::list<BehaviourComponent*>& BehaviourSystem::getUpdateQueue()
{
	return updateQueue;
}

std::list<BehaviourComponent*> BehaviourSystem::updateQueue;
