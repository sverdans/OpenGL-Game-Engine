#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <General/Constants.h>

#include <Resources/ResourceManager.h>

#include <System/InputHandler.h>
#include <System/Window.h>
#include <System/Time.h>

#include <Game/GameObject.h>
#include <Game/Components/CameraComponent.h>
#include <Game/Components/BehaviourComponent.h>
#include <Game/Components/TransformComponent.h>


CameraComponent::CameraComponent(GameObject* pGameObject) 
	: BehaviourComponent(pGameObject) 
{ }

void CameraComponent::UpdateViewMatrix()
{
	auto pTC = GetGameObject()->GetComponent<TransformComponent>();
	glm::vec3 position = pTC->GetGlobalPosition();
	viewMatrix = glm::lookAt(position, position + pTC->GetForward(), constants::WorldUp);
}

void CameraComponent::UpdateProjectionMatrix()
{
	float aspectRatio = viewportWidth / viewportHeight;
	if (aspectRatio != aspectRatio)
		return;

	if (projectionMode == ProjectionMode::Perspective)
	{
		projectionMatrix = glm::perspective(
			glm::radians(fieldOfView),
			aspectRatio,
			nearClipPlane,
			farClipPlane);
	}
	else if (projectionMode == ProjectionMode::Orthographic)
	{
		projectionMatrix = glm::ortho(
			-orthoScale * aspectRatio,
			orthoScale * aspectRatio,
			-orthoScale,
			orthoScale,
			nearClipPlane,
			farClipPlane);
	}
	else
	{
		std::cout << "warning" << std::endl;
	}
}

void CameraComponent::Init() 
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void CameraComponent::Update()
{
	if (!isMovable)
		return;

	viewportWidth = Window::size.x;
	viewportHeight = Window::size.y;

	auto pTC = GetGameObject()->GetComponent<TransformComponent>();
	auto& keys = InputHandler::getKeys();
	
	float delta = 0.002f * Time::getDeltaTime();
	float deltaR = 0.05f * Time::getDeltaTime();

	glm::vec3 movementDelta(0.f, 0.f, 0.f);
	glm::vec3 rotationDelta(0.f, 0.f, 0.f);

	if (keys[GLFW_KEY_W]) movementDelta.z += delta;
	if (keys[GLFW_KEY_S]) movementDelta.z -= delta;
	
	if (keys[GLFW_KEY_A]) movementDelta.x += delta;	// -= ?
	if (keys[GLFW_KEY_D]) movementDelta.x -= delta; // += ?
	
	if (keys[GLFW_KEY_E]) movementDelta.y += delta;
	if (keys[GLFW_KEY_Q]) movementDelta.y -= delta;

	if (keys[GLFW_KEY_RIGHT]) rotationDelta.y -= deltaR;
	if (keys[GLFW_KEY_LEFT])  rotationDelta.y += deltaR;

	if (keys[GLFW_KEY_UP])   rotationDelta.x -= deltaR;
	if (keys[GLFW_KEY_DOWN]) rotationDelta.x += deltaR;

	pTC->SetRotation(pTC->GetRotation() + rotationDelta);
	pTC->Translate(pTC->GetForward() * movementDelta.z);
	pTC->Translate(pTC->GetRight() * movementDelta.x);
	pTC->Translate(pTC->GetUp() * movementDelta.y);
}

void CameraComponent::SetProjectionMode(ProjectionMode mode)
{
	projectionMode = mode;
	UpdateProjectionMatrix();
}

void CameraComponent::OnPreRender()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();

	viewProjectionMatrix = projectionMatrix * viewMatrix;
	
	if (auto pDefaultShader = ResourceManager::getShader("defaultShader"))
	{
		pDefaultShader->use();
		pDefaultShader->setMatrix("viewProjectionMat", viewProjectionMatrix);
		pDefaultShader->setVec3("viewPosition", GetGameObject()->GetComponent<TransformComponent>()->GetGlobalPosition());
		pDefaultShader->unuse();
	}

	if (auto pSimpleShader = ResourceManager::getShader("simpleShader"))
	{
		pSimpleShader->use();
		pSimpleShader->setMatrix("viewProjectionMat", viewProjectionMatrix);
		pSimpleShader->unuse();
	}
}

void CameraComponent::Deserialize(const nlohmann::json& jsonObject)
{
	viewportWidth = Window::size.x;
	viewportHeight = Window::size.y;

	isMovable      = jsonObject["isMovable"];
	fieldOfView    = jsonObject["fieldOfView"];
	farClipPlane   = jsonObject["farClipPlane"];
	nearClipPlane  = jsonObject["nearClipPlane"];
	projectionMode = jsonObject["ProjectionMode"];

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void CameraComponent::Serialize(nlohmann::json& jsonObject)
{
	jsonObject[Name()] = {
		{ "isMovable",      isMovable      },
		{ "fieldOfView",    fieldOfView    },
		{ "farClipPlane",   farClipPlane   },
		{ "nearClipPlane",  nearClipPlane  },
		{ "ProjectionMode", projectionMode },
	};
}
