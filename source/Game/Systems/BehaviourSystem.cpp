#include <Game/Systems/BehaviourSystem.h>
#include <Game/Components/BehaviourComponent.h>

void BehaviourSystem::AddToUpdateQueue(BehaviourComponent* pComponent)
{
	mUpdateQueue.push_back(pComponent);
}

void BehaviourSystem::RemoveFromUpdateQueue(BehaviourComponent* pComponent)
{
	mUpdateQueue.remove(pComponent);
}

void BehaviourSystem::Init()
{
	for (auto it : mUpdateQueue)
		it->Init();
}

void BehaviourSystem::Update()
{
	std::cout << mUpdateQueue.size() << std::endl;
	for (auto it : mUpdateQueue)
		it->Update();
}

void BehaviourSystem::LateUpdate()
{
	for (auto it : mUpdateQueue)
		it->LateUpdate();
}

void BehaviourSystem::OnPreRender()
{
	for (auto it : mUpdateQueue)
		it->OnPreRender();
}


std::list<BehaviourComponent*>& BehaviourSystem::getUpdateQueue()
{
	return mUpdateQueue;
}
