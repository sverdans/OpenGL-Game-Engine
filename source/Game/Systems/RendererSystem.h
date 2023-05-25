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

	static void addToRenderQueue(RendererComponent* RC);
	static void removeFromRenderQueue(RendererComponent* RC);
	static void render();

	static std::list<RendererComponent*>& getRenderQueue();
};
