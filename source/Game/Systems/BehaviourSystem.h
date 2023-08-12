#pragma once
#include <list>

class BehaviourComponent;

class BehaviourSystem final
{
private:
	friend class Editor;

	static std::list<BehaviourComponent*> updateQueue;

public:
	BehaviourSystem() = delete;
	~BehaviourSystem() = delete;
	BehaviourSystem(const BehaviourSystem&) = delete;
	BehaviourSystem& operator = (const BehaviourSystem&) = delete;

	static void addToUpdateQueue(BehaviourComponent* BC);
	static void removeFromUpdateQueue(BehaviourComponent* BC);
	
	static void init();
	static void update();
	static void lateUpdate();
	static void onPreRender();

	static std::list<BehaviourComponent*>& getUpdateQueue();
};
