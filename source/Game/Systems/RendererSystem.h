#pragma once
#include <list>

class RendererComponent;

class RendererSystem final
{
	friend class Editor;
public:
    static RendererSystem& Instance() 
    { 
        static RendererSystem instance;
        return instance;
    }

	void AddToRenderQueue(RendererComponent* pComponent);
	void RemoveFromRenderQueue(RendererComponent* pComponent);
	
	void Render();
	
	std::list<RendererComponent*>& GetRenderQueue();

private:
	RendererSystem() = default;
	~RendererSystem() = default;
	RendererSystem(const RendererSystem&) = delete;
	RendererSystem(const RendererSystem&&) = delete;
	RendererSystem& operator = (const RendererSystem&) = delete;
	RendererSystem& operator = (const RendererSystem&&) = delete;

private:
	std::list<RendererComponent*> mRenderQueue;
};
