#include <Game/Systems/RendererSystem.h>
#include <Game/Components/RendererComponent.h>

void RendererSystem::AddToRenderQueue(RendererComponent* RC)
{
	renderQueue.push_back(RC);
}

void RendererSystem::RemoveFromRenderQueue(RendererComponent* RC)
{
	renderQueue.remove(RC);
}

void RendererSystem::Render()
{
	for (auto it : renderQueue)
	{
		it->Update();
		it->Render();
	}
}

std::list<RendererComponent*>& RendererSystem::GetRenderQueue()
{
	return renderQueue;
}


std::list<RendererComponent*> RendererSystem::renderQueue;
