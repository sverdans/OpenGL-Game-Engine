#pragma once
#include <list>
#include <glm/vec3.hpp>

#include <Game/ResourceManager.h>

class LightingComponent;

class LightingSystem final
{
private:
	friend class Editor;
	
	static float ambientStrength;
	static glm::vec3 ambientColor;

	static constexpr unsigned int maxDirLights = 4;
	static constexpr unsigned int maxPointLights = 8;

	static std::list<LightingComponent*> lightingComponents;

public:
	LightingSystem() = delete;
	~LightingSystem() = delete;
	LightingSystem(const LightingSystem&) = delete;
	LightingSystem& operator = (const LightingSystem&) = delete;

	static const glm::vec3& getAmbientColor() { return ambientColor; }
	static const float getAmbientStrength() { return ambientStrength; }

	static void setAmbientColor(const glm::vec3& color) { ambientColor = color; }
	static void setAmbientStrength(const float strength) { ambientStrength = strength; }

	static void addLightingComponent(LightingComponent* LC);
	static void removeLightingComponent(LightingComponent* LC);

	static void update();
};
