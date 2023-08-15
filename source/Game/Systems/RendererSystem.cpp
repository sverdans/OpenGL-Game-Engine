#include <Game/Systems/RendererSystem.h>
#include <Game/Components/RendererComponent.h>

void RendererSystem::AddToRenderQueue(RendererComponent* pComponent)
{
	mRenderQueue.push_back(pComponent);
}

void RendererSystem::RemoveFromRenderQueue(RendererComponent* pComponent)
{
	mRenderQueue.remove(pComponent);
}

void RendererSystem::Render()
{
	for (auto it : mRenderQueue)
	{
		it->Update();
		it->Render();
	}
}

std::list<RendererComponent*>& RendererSystem::GetRenderQueue()
{
	return mRenderQueue;
}
