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
	enum EnProjectionMode
	{
		ePerspective,
		eOrthographic,
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

	void SetProjectionMode(EnProjectionMode eMode);

	std::string Name() override;
	nlohmann::json Serialize() override;
	void Deserialize(const nlohmann::json& jsonObject) override;

private:
	EnProjectionMode meProjectionMode;

	glm::mat4 viewMatrix = glm::mat4(1.0f);
	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	glm::mat4 viewProjectionMatrix = glm::mat4(1.0f);

	bool mlIsMovable;

	float mOrthoScale;
	float mFarClipPlane;
	float mNearClipPlane;
	float mViewportWidth;
	float mViewportHeight;
	float mFieldOfView;
};

