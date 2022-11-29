#include "LightingSystem.h"
#include "../GameObject.h"
#include "../Components/LightingComponent.h"
#include "../Components/TransformComponent.h"

void LightingSystem::addLightingComponent(LightingComponent* LC)
{
	lightingComponents.push_back(LC);
}

void LightingSystem::removeLightingComponent(LightingComponent* LC)
{
	lightingComponents.remove(LC);
}

void LightingSystem::update()
{
	auto ds = ResourceManager::getShader("defaultShader");
	if (ds)
	{
		ds->use();
		ds->setFloat("ambientStrength", ambientStrength);
		ds->setVec3("ambientColor", ambientColor);

		auto LC = lightingComponents.size() != 0 ? lightingComponents.front() : nullptr;
		if (LC)
		{
			ds->setVec3("lightColor", LC->color);
			ds->setVec3("lightPosition", LC->gameObject->getComponent<TransformComponent>()->getGlobalPosition());
		}
		
		ds->unuse();
	}
}

float LightingSystem::ambientStrength = 0.1;
glm::vec3 LightingSystem::ambientColor = glm::vec3(1.f, 1.f, 1.f);
std::list<LightingComponent*> LightingSystem::lightingComponents;