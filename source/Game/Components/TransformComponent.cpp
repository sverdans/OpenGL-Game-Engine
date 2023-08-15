#include <Game/Components/TransformComponent.h>

#include <iostream>

#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Game/GameObject.h>
#include <General/Constants.h>

DECLARE_COMPONENT(TransformComponent)

TransformComponent::TransformComponent(GameObject* pGameObject)
	: Component(pGameObject) 
{ }

const glm::vec3& TransformComponent::SetPosition(const glm::vec3& position)
{
	return mPosition = position;
}

const glm::vec3& TransformComponent::SetRotation(const glm::vec3& rotation)
{
	const float rollInRadians  = glm::radians(rotation.x);
	const float pitchInRadians = glm::radians(rotation.y);
	const float yawInRadians   = glm::radians(rotation.z);

	const glm::mat3 rotateMatrixX
	{
		1, 0                  , 0                 ,
		0, cos(rollInRadians) , sin(rollInRadians),
		0, -sin(rollInRadians), cos(rollInRadians),
	};

	const glm::mat3 rotateMatrixY
	{
		cos(pitchInRadians), 0, -sin(pitchInRadians),
		0                  , 1, 0                   ,
		sin(pitchInRadians), 0, cos(pitchInRadians) ,
	};

	const glm::mat3 rotateMatrixZ
	{
		cos(yawInRadians) , sin(yawInRadians), 0,
		-sin(yawInRadians), cos(yawInRadians), 0,
		0                 ,                 0, 1,
	};

	const glm::mat3 eulerRotateMatrix = rotateMatrixZ * rotateMatrixY * rotateMatrixX;
	
	mForward = glm::normalize(eulerRotateMatrix * constants::WorldForward);
	mRight   = glm::normalize(eulerRotateMatrix * constants::WorldRight);
	mUp      = glm::cross(mRight, mForward);

	return mRotation = rotation;
}
const glm::vec3& TransformComponent::SetSize(const glm::vec3& size) 
{ 
	return mSize = size;
}

const glm::vec3& TransformComponent::Translate(const glm::vec3& vec) 
{ 
	return mPosition += vec;
}

const glm::vec3& TransformComponent::Translate(const glm::vec3& direction, float distance)
{
	return mPosition += glm::normalize(direction) * distance;
}

const glm::vec3& TransformComponent::Rotate(const glm::vec3& axis, float angle)
{
	mRotation.x += angle * ((axis.x > 0) - (axis.x < 0));
	mRotation.y += angle * ((axis.y > 0) - (axis.y < 0));
	mRotation.z += angle * ((axis.z > 0) - (axis.z < 0));
	return mRotation;
}
const glm::vec3& TransformComponent::Scale(const glm::vec3& sc)
{
	mSize.x *= sc.x;
	mSize.y *= sc.y;
	mSize.z *= sc.z;
	return mSize;
}

const glm::vec3 TransformComponent::GetGlobalPosition() const
{
	glm::vec3 globalPosition(0, 0, 0);
	auto pObject = GetGameObject();
	do
	{
		auto TC = pObject->GetComponent<TransformComponent>();
		globalPosition += TC->mPosition;
	}
	while (pObject = pObject->GetParent());

	return globalPosition;
}
const glm::vec3 TransformComponent::GetGlobalRotation() const
{
	glm::vec3 globalRotation(0, 0, 0);
	auto pObject = GetGameObject();
	do
	{
		auto TC = pObject->GetComponent<TransformComponent>();
		globalRotation += TC->mRotation;
	}
	while (pObject = pObject->GetParent());

	return globalRotation;
}

const glm::vec3& TransformComponent::GetPosition() const { return mPosition; }
const glm::vec3& TransformComponent::GetRotation() const { return mRotation; }
const glm::vec3& TransformComponent::GetSize() const     { return mSize;     }
const glm::vec3& TransformComponent::GetForward() const  { return mForward;  }
const glm::vec3& TransformComponent::GetRight() const    { return mRight;    }
const glm::vec3& TransformComponent::GetUp() const       { return mUp;       }

const glm::mat4x4& TransformComponent::GetModelMatrix() const
{
	auto pParent = GetGameObject()->GetParent();
	mModel = pParent ? pParent->GetComponent<TransformComponent>()->GetModelMatrix() : glm::mat4(1.f);

	mModel = glm::scale(mModel, mSize);
	mModel = glm::translate(mModel, mPosition / mSize);
	mModel = glm::rotate(mModel, glm::radians(mRotation.z), constants::WorldForward);
	mModel = glm::rotate(mModel, glm::radians(mRotation.y), constants::WorldUp);
	mModel = glm::rotate(mModel, glm::radians(mRotation.x), constants::WorldRight);

	return mModel;
}

void TransformComponent::Deserialize(const nlohmann::json& jsonObject)
{
	const auto& jsonPosition = jsonObject["position"];
	SetPosition({
		jsonPosition["x"],
		jsonPosition["y"],
		jsonPosition["z"] 
	});

	const auto& jsonRotation = jsonObject["rotation"];
	SetRotation({
		jsonRotation["x"],
		jsonRotation["y"],
		jsonRotation["z"]
	});

	const auto& jsonSize = jsonObject["size"];
	SetSize({
		jsonSize["x"],
		jsonSize["y"],
		jsonSize["z"]
	});
}

nlohmann::json TransformComponent::Serialize()
{
	nlohmann::json jsonPosition {
		{ "x", mPosition.x },
		{ "y", mPosition.y },
		{ "z", mPosition.z },
	};

	nlohmann::json jsonRotation {
		{ "x", mRotation.x },
		{ "y", mRotation.y },
		{ "z", mRotation.z },
	};
	
	nlohmann::json jsonSize {
		{ "x", mSize.x },
		{ "y", mSize.y },
		{ "z", mSize.z },
	};

	return {
		{ "position", jsonPosition },
		{ "rotation", jsonRotation },
		{ "size", jsonSize },
	};
}