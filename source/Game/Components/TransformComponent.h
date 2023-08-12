#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <Game/Component.h>
#include <Game/ObjectsManager.h>

class TransformComponent : public Component
{
public:
	TransformComponent(GameObject* gameObject);

	TransformComponent() = delete;
	TransformComponent(const TransformComponent&) = delete;
	TransformComponent& operator = (const TransformComponent&) = delete;
	~TransformComponent() = default;

	const glm::vec3& SetPosition(const glm::vec3& position);
	const glm::vec3& SetRotation(const glm::vec3& rotation);
	const glm::vec3& SetSize(const glm::vec3& size);

	const glm::vec3& Translate(const glm::vec3& vec);
	const glm::vec3& Translate(const glm::vec3& direction, float distance);

	const glm::vec3& Rotate(const glm::vec3& axis, float angle);
	const glm::vec3& Scale(const glm::vec3& sc);

	const glm::vec3& GetSize() const;
	const glm::vec3& GetRotation() const;
	const glm::vec3& GetPosition() const;

	const glm::vec3 GetGlobalPosition() const;
	const glm::vec3 GetGlobalRotation() const;

	const glm::vec3& GetForward() const;
	const glm::vec3& GetRight() const;
	const glm::vec3& GetUp() const;

	const glm::mat4x4& GetModelMatrix() const;

	void Deserialize(const nlohmann::json& jsonObject) override;
	void Serialize(nlohmann::json& jsonObject) override;
	std::string Name() override { return "TransformComponent"; }

protected:
	glm::vec3 mPosition = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 mRotation = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 mSize     = glm::vec3(0.f, 0.f, 0.f);

	glm::vec3 mRight   = glm::vec3(1.f, 0.f, 0.f);
	glm::vec3 mUp      = glm::vec3(0.f, 1.f, 0.f);
	glm::vec3 mForward = glm::vec3(0.f, 0.f, 1.f);

	mutable glm::mat4 mModel = glm::mat4(1.f);
};
