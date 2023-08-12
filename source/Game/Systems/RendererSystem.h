#pragma once
#include <list>

class RendererComponent;

class RendererSystem final
{
private:
	friend class Editor;

	static std::list<RendererComponent*> renderQueue;

public:
	RendererSystem() = delete;
	~RendererSystem() = delete;
	RendererSystem(const RendererSystem&) = delete;
	RendererSystem& operator = (const RendererSystem&) = delete;

	static void AddToRenderQueue(RendererComponent* RC);
	static void RemoveFromRenderQueue(RendererComponent* RC);
	
	static void Render();

	static std::list<RendererComponent*>& GetRenderQueue();
};
