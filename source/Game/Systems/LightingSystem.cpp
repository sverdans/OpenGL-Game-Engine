#include <Game/Systems/LightingSystem.h>
#include <Game/GameObject.h>
#include <Game/Components/LightingComponent.h>
#include <Game/Components/TransformComponent.h>

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
	std::list<ShaderProgram*> shaders;
	auto ds = ResourceManager::getShader("defaultShader");
	if (ds) shaders.push_back(ds);

	for (auto& shader : shaders)
	{
		shader->use();
		shader->setFloat("ambientStrength", ambientStrength);
		shader->setVec3("ambientColor", ambientColor);

		unsigned int dirIndex = 0, pointIndex = 0;
		for (auto& it : lightingComponents)
		{
			switch (it->meType)
			{
				case LightingComponent::Type::Directional:
				{
					shader->setVec3("directionalLights[" + std::to_string(dirIndex) + "].color", it->mColor);
					shader->setFloat("directionalLights[" + std::to_string(dirIndex) + "].intensity", it->mIntensity);
					shader->setVec3("directionalLights[" + std::to_string(dirIndex) + "].direction", -it->GetGameObject()->GetComponent<TransformComponent>()->GetForward());
					dirIndex++;
					break;
				}
					
				case LightingComponent::Type::Point:
				{
					shader->setVec3("pointLights[" + std::to_string(pointIndex) + "].color", it->mColor);
					shader->setFloat("pointLights[" + std::to_string(pointIndex) + "].intensity", it->mIntensity);
					shader->setVec3("pointLights[" + std::to_string(pointIndex) + "].position", it->GetGameObject()->GetComponent<TransformComponent>()->GetGlobalPosition());
					pointIndex++;
					break;
				}
			}
		}

		for (unsigned int i = dirIndex; i < maxDirLights; ++i)
		{
			shader->setVec3("directionalLights[" + std::to_string(i) + "].color", glm::vec3(0, 0, 0));
			shader->setFloat("directionalLights[" + std::to_string(i) + "].intensity", 0.f);
			shader->setVec3("directionalLights[" + std::to_string(i) + "].direction", glm::vec3(0, 0, 0));
		}

		for (unsigned int i = pointIndex; i < maxPointLights; ++i)
		{
			shader->setVec3("pointLights[" + std::to_string(i) + "].color", glm::vec3(0, 0, 0));
			shader->setFloat("pointLights[" + std::to_string(i) + "].intensity", 0.f);
			shader->setVec3("pointLights[" + std::to_string(i) + "].position", glm::vec3(0, 0, 0));
		}

		shader->unuse();
	}
}

float LightingSystem::ambientStrength = 0.4;
glm::vec3 LightingSystem::ambientColor = glm::vec3(1.f, 1.f, 1.f);
std::list<LightingComponent*> LightingSystem::lightingComponents;