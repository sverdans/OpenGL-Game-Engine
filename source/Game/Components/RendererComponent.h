#pragma once
#include <glm/vec2.hpp>
#include <functional>

#include <Game/Component.h>
#include <Game/ResourceManager.h>
#include <System/Time.h>

class RendererComponent : public Component
{
public:
	bool mlRenderEnable;

	RendererComponent() = delete;
	RendererComponent(const RendererComponent&) = delete;
	RendererComponent& operator = (const RendererComponent&) = delete;

	RendererComponent(GameObject* pGameObject);
	~RendererComponent();

	virtual void Render() = 0;
	virtual void Update() {};
};
