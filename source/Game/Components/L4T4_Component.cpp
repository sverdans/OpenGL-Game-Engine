#include "L4T4_Component.h"

#include "../ObjectsManager.h"
#include "../../System/InputHandler.h"

#include "GLFW/glfw3.h"

void L4T4_Component::init()
{
	focusGameObject = ObjectsManager::findByName("Cube");
	auto focusPos = focusGameObject->getComponent<TransformComponent>()->getGlobalPosition();
	auto TC = gameObject->getComponent<TransformComponent>();
	
	float deltaB = Time::getDeltaTime() * velocity * delta; 

	TC->setPosition(glm::vec3(
		focusPos.x + acos(a) * radius,
		focusPos.y,
		focusPos.z + asin(a) * radius
	));
}

void L4T4_Component::update()
{
	auto& keys = InputHandler::getKeys();

	if (keys[GLFW_KEY_EQUAL])
		velocity += delta;

	if (keys[GLFW_KEY_MINUS])
		velocity -= delta;

	auto focusPos = focusGameObject->getComponent<TransformComponent>()->getGlobalPosition();
	auto TC = gameObject->getComponent<TransformComponent>();

	float deltaB = Time::getDeltaTime() * velocity * delta;
	a += deltaB;

	TC->setPosition(glm::vec3(
		focusPos.x + cos(a) * radius,
		focusPos.y,
		focusPos.z + sin(a) * radius
	));
}