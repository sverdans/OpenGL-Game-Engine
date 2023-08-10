#pragma once
#include <Game/Component.h>
#include <Game/ObjectsManager.h>

class GameObject;

class BehaviourComponent : public Component
{
public:
	BehaviourComponent() = delete;
	BehaviourComponent(const BehaviourComponent&) = delete;
	BehaviourComponent& operator = (const BehaviourComponent&) = delete;

	BehaviourComponent(GameObject* pGameObject);
	virtual ~BehaviourComponent();

	virtual void Init()        { }
	virtual void Update()      { }
	virtual void LateUpdate()  { }
	virtual void OnPreRender() { }
};
