#pragma once
#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <System/Window.h>
#include <Game/Components/BehaviourComponent.h>

class GameObject;

class CameraComponent : public BehaviourComponent
{
public:
	enum ProjectionMode
	{
		Perspective,
		Orthographic
	};

public:
	CameraComponent(GameObject* gameObject);
	CameraComponent(const CameraComponent&) = delete;
	CameraComponent& operator = (const CameraComponent&) = delete;
	~CameraComponent() = default;

	void UpdateViewMatrix();
	void UpdateProjectionMatrix();

	void Init() override;
	void Update() override;
	void OnPreRender() override;

	void SetProjectionMode(ProjectionMode mode);

	void Deserialize(const nlohmann::json& jsonObject) override;
	void Serialize(nlohmann::json& jsonObject) override;

	std::string Name() override { return "CameraComponent"; }

private:
	ProjectionMode projectionMode;

	glm::mat4 viewMatrix = glm::mat4(1.0f);
	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	glm::mat4 viewProjectionMatrix = glm::mat4(1.0f);

	bool isMovable;

	float orthoScale = 1;

	float farClipPlane;
	float nearClipPlane;

	float viewportWidth = Window::size.x;
	float viewportHeight = Window::size.y;
	float fieldOfView;
};

//DECLARE_COMPONENT(CameraComponent) 
