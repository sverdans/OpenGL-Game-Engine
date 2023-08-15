#pragma once
#include <list>
#include <glm/vec3.hpp>

#include <Game/ResourceManager.h>

class LightingComponent;

class LightingSystem final
{
	friend class Editor;
public:
    static LightingSystem& Instance() 
    { 
        static LightingSystem instance;
        return instance;
    }

	void AddLightingComponent(LightingComponent* LC);
	void RemoveLightingComponent(LightingComponent* LC);

	const glm::vec3& getAmbientColor() { return ambientColor; }
	const float getAmbientStrength() { return ambientStrength; }
	void setAmbientColor(const glm::vec3& color) { ambientColor = color; }
	void setAmbientStrength(const float strength) { ambientStrength = strength; }
	void Update();

private:
	LightingSystem() = default;
	~LightingSystem() = default;
	LightingSystem(const LightingSystem&) = delete;
	LightingSystem(const LightingSystem&&) = delete;
	LightingSystem& operator = (const LightingSystem&) = delete;
	LightingSystem& operator = (const LightingSystem&&) = delete;

private:
	std::list<LightingComponent*> mLightingComponents;
	
	float ambientStrength;
	glm::vec3 ambientColor;

	const unsigned maxDirLights = 4;
	const unsigned maxPointLights = 8;
};
