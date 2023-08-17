#include <glm/mat4x4.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <General/Constants.h>

#include <Game/ResourceManager.h>

#include <System/InputHandler.h>
#include <System/Window.h>
#include <System/Time.h>

#include <Game/GameObject.h>
#include <Game/Components/CameraComponent.h>
#include <Game/Components/BehaviourComponent.h>
#include <Game/Components/TransformComponent.h>

DECLARE_COMPONENT(CameraComponent)

CameraComponent::CameraComponent(GameObject* pGameObject) 
	: BehaviourComponent(pGameObject)
	, mOrthoScale(1)
	, mViewportWidth(Window::Instance().GetWidth())
	, mViewportHeight(Window::Instance().GetHeight())
{ }

void CameraComponent::UpdateViewMatrix()
{
	auto pTC = GetGameObject()->GetComponent<TransformComponent>();
	if (!pTC)
		return;

	glm::vec3 position = pTC->GetGlobalPosition();
	viewMatrix = glm::lookAt(position, position + pTC->GetForward(), constants::WorldUp);
}

void CameraComponent::UpdateProjectionMatrix()
{
	float aspectRatio = mViewportWidth / mViewportHeight;
	if (aspectRatio != aspectRatio)
		return;

	switch (meProjectionMode)
	{
		case EnProjectionMode::ePerspective: 
		{
			projectionMatrix = glm::perspective(
				glm::radians(mFieldOfView),
				aspectRatio,
				mNearClipPlane,
				mFarClipPlane);
			break;
		}
		case EnProjectionMode::eOrthographic:
		{
			projectionMatrix = glm::ortho(
				-mOrthoScale * aspectRatio,
				 mOrthoScale * aspectRatio,
				-mOrthoScale,
				 mOrthoScale,
				mNearClipPlane,
				mFarClipPlane);
			break;
		}
		default:
		{
			std::cout << "warning" << std::endl;
			break;
		}
	}
}

void CameraComponent::Init() 
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();
}

void CameraComponent::Update()
{
	if (!mlIsMovable)
		return;

	mViewportWidth  = Window::Instance().GetWidth();
	mViewportHeight = Window::Instance().GetHeight();

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

void CameraComponent::SetProjectionMode(EnProjectionMode eMode)
{
	meProjectionMode = eMode;
	UpdateProjectionMatrix();
}

void CameraComponent::OnPreRender()
{
	UpdateViewMatrix();
	UpdateProjectionMatrix();

	viewProjectionMatrix = projectionMatrix * viewMatrix;
	
	if (auto pDefaultShader = ResourceManager::Instance().GetShader("defaultShader"))
	{
		pDefaultShader->use();
		pDefaultShader->setMatrix("viewProjectionMat", viewProjectionMatrix);
		pDefaultShader->setVec3("viewPosition", GetGameObject()->GetComponent<TransformComponent>()->GetGlobalPosition());
		pDefaultShader->unuse();
	}

	if (auto pSimpleShader = ResourceManager::Instance().GetShader("simpleShader"))
	{
		pSimpleShader->use();
		pSimpleShader->setMatrix("viewProjectionMat", viewProjectionMatrix);
		pSimpleShader->unuse();
	}
}

Component* CameraComponent::Clone() { return new CameraComponent(nullptr); }
std::string CameraComponent::Name() { return "CameraComponent"; }

nlohmann::json CameraComponent::Serialize()
{
	return {
		{ "isMovable",      mlIsMovable      },
		{ "fieldOfView",    mFieldOfView     },
		{ "farClipPlane",   mFarClipPlane    },
		{ "nearClipPlane",  mNearClipPlane   },
		{ "ProjectionMode", meProjectionMode },
	};
}

void CameraComponent::Deserialize(const nlohmann::json& jsonObject)
{
	mViewportWidth  = Window::Instance().GetWidth();
	mViewportHeight = Window::Instance().GetHeight();

	mlIsMovable      = jsonObject["isMovable"];
	mFieldOfView     = jsonObject["fieldOfView"];
	mFarClipPlane    = jsonObject["farClipPlane"];
	mNearClipPlane   = jsonObject["nearClipPlane"];
	meProjectionMode = jsonObject["ProjectionMode"];

	UpdateViewMatrix();
	UpdateProjectionMatrix();
}
