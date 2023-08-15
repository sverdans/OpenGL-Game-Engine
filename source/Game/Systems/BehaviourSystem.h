#pragma once
#include <list>

class BehaviourComponent;

class BehaviourSystem final
{
	friend class Editor;
public:
	static BehaviourSystem& Instance() 
    { 
        static BehaviourSystem instance;
        return instance;
    }
	
	void AddToUpdateQueue(BehaviourComponent* pComponent);
	void RemoveFromUpdateQueue(BehaviourComponent* pComponent);
	
	void Init();
	void Update();
	void LateUpdate();
	void OnPreRender();

	std::list<BehaviourComponent*>& getUpdateQueue();

private:
	BehaviourSystem() = default;
	~BehaviourSystem() = default;
	BehaviourSystem(const BehaviourSystem&) = delete;
	BehaviourSystem(const BehaviourSystem&&) = delete;
	BehaviourSystem& operator = (const BehaviourSystem&) = delete;
	BehaviourSystem& operator = (const BehaviourSystem&&) = delete;

private:
	std::list<BehaviourComponent*> mUpdateQueue;
};
