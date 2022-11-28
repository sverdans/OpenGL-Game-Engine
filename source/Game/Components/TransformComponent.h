#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "Component.h"
#include "../GameObject.h"

class TransformComponent : public Component
{
protected:
	glm::vec3 position = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 rotation = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 size     = glm::vec3(0.f, 0.f, 0.f);

	glm::vec3 right   = glm::vec3(1.f, 0.f, 0.f);
	glm::vec3 up      = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 forward = glm::vec3(0.f, 0.f, 1.f);

	mutable glm::mat4 model = glm::mat4(1.f);

public:
	TransformComponent() = delete;
	TransformComponent(const TransformComponent&) = delete;
	TransformComponent& operator = (const TransformComponent&) = delete;
	~TransformComponent() = default;

	TransformComponent(GameObject* gameObject);
	
	const glm::vec3& setPosition(const glm::vec3& position);
	const glm::vec3& setRotation(const glm::vec3& rotation);
	const glm::vec3& setSize(const glm::vec3& size);

	const glm::vec3& translate(const glm::vec3& vec);
	const glm::vec3& translate(const glm::vec3& direction, float distance);

	const glm::vec3& rotate(const glm::vec3& axis, float angle);
	const glm::vec3& scale(const glm::vec3& sc);

	const glm::vec3& getSize() const;
	const glm::vec3& getRotation() const;
	const glm::vec3& getPosition() const;
	const glm::vec3 getGlobalPosition() const;

	const glm::vec3& getForward() const;
	const glm::vec3& getRight() const;
	const glm::vec3& getUp() const;

	const glm::mat4x4& getModelMatrix() const;

	void deserialize(const nlohmann::json& jsonObject) override;
	void serialize(nlohmann::json& jsonObject) override;
};