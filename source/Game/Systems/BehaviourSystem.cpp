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
	{
		if (it->GetGameObject())
		{
			it->Init();
		}
	}
}

void BehaviourSystem::Update()
{
	for (auto it : mUpdateQueue)
	{
		if (it->GetGameObject())
		{
			it->Update();
		}
	}
}

void BehaviourSystem::LateUpdate()
{
	for (auto it : mUpdateQueue)
	{
		if (it->GetGameObject())
		{
			it->LateUpdate();
		}
	}
}

void BehaviourSystem::OnPreRender()
{
	for (auto it : mUpdateQueue)
	{
		if (it->GetGameObject())
		{
			it->OnPreRender();
		}
	}
}


std::list<BehaviourComponent*>& BehaviourSystem::getUpdateQueue()
{
	return mUpdateQueue;
}
