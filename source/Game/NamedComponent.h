#include <Game/Component.h>

template<class T>
class NamedComponent : public Component
{
public:
	NamedComponent(GameObject* pGameObject) 
		: Component(pGameObject) { }
	virtual ~NamedComponent() { }

	Component* Clone() override
	{
		return new T(nullptr);
	}
	std::string Name() override
	{
		return typeid(T).name();
	}
};
