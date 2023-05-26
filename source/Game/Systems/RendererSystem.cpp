#include <Game/Systems/RendererSystem.h>
#include <Game/Components/RendererComponent.h>

void RendererSystem::addToRenderQueue(RendererComponent* RC)
{
	renderQueue.push_back(RC);
}

void RendererSystem::removeFromRenderQueue(RendererComponent* RC)
{
	renderQueue.remove(RC);
}

void RendererSystem::render()
{
	for (auto it : renderQueue)
		it->update(), it->render();
}

std::list<RendererComponent*>& RendererSystem::getRenderQueue()
{
	return renderQueue;
}


std::list<RendererComponent*> RendererSystem::renderQueue;
