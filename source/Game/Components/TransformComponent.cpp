#include "TransformComponent.h"

#include <iostream>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../GameObject.h"
#include "../../System/Constants.h"

TransformComponent::TransformComponent(GameObject* gameObject) 
	: Component(gameObject) {}

const glm::vec3& TransformComponent::setPosition(const glm::vec3& position)
{
	return this->position = position;
}
const glm::vec3& TransformComponent::setRotation(const glm::vec3& rotation)
{
	const float rollInRadians  = glm::radians(rotation.x);
	const float pitchInRadians = glm::radians(rotation.y);
	const float yawInRadians   = glm::radians(rotation.z);

	const glm::mat3 rotateMatrixX(
		1, 0, 0,
		0, cos(rollInRadians), sin(rollInRadians),
		0, -sin(rollInRadians), cos(rollInRadians));

	const glm::mat3 rotateMatrixY(
		cos(pitchInRadians), 0, -sin(pitchInRadians),
		0, 1, 0,
		sin(pitchInRadians), 0, cos(pitchInRadians));

	const glm::mat3 rotateMatrixZ(
		cos(yawInRadians), sin(yawInRadians), 0,
		-sin(yawInRadians), cos(yawInRadians), 0,
		0, 0, 1);

	const glm::mat3 eulerRotateMatrix = rotateMatrixZ * rotateMatrixY * rotateMatrixX;
	
	forward = glm::normalize(eulerRotateMatrix * constants::worldForward);
	right = glm::normalize(eulerRotateMatrix * constants::worldRight);
	up = glm::cross(right, forward);

	return this->rotation = rotation;
}
const glm::vec3& TransformComponent::setSize(const glm::vec3& size) 
{ 
	return this->size = size;
}

const glm::vec3& TransformComponent::translate(const glm::vec3& vec) { return position += vec; }
const glm::vec3& TransformComponent::translate(const glm::vec3& direction, float distance)
{
	return position += glm::normalize(direction) * distance;
}

const glm::vec3& TransformComponent::rotate(const glm::vec3& axis, float angle)
{
	rotation.x += angle * ((axis.x > 0) - (axis.x < 0));
	rotation.y += angle * ((axis.y > 0) - (axis.y < 0));
	rotation.z += angle * ((axis.z > 0) - (axis.z < 0));
	return rotation;
}
const glm::vec3& TransformComponent::scale(const glm::vec3& sc)
{
	size.x *= sc.x;
	size.y *= sc.y;
	size.z *= sc.z;
	return this->size;
}

const glm::vec3 TransformComponent::getGlobalPosition() const
{
	glm::vec3 globalPosition(0, 0, 0);

	auto object = gameObject;
	do
	{
		auto TC = object->getComponent<TransformComponent>();
		globalPosition += TC->position;
	} while (object = object->getParent());

	return globalPosition;
}
const glm::vec3 TransformComponent::getGlobalRotation() const
{
	glm::vec3 globalRotation(0, 0, 0);

	auto object = gameObject;
	do
	{
		auto TC = object->getComponent<TransformComponent>();
		globalRotation += TC->rotation;
	} while (object = object->getParent());

	return globalRotation;
}

const glm::vec3& TransformComponent::getPosition() const { return position; }
const glm::vec3& TransformComponent::getRotation() const { return rotation; }
const glm::vec3& TransformComponent::getSize() const { return size; }

const glm::vec3& TransformComponent::getForward() const { return forward; }
const glm::vec3& TransformComponent::getRight() const { return right; }
const glm::vec3& TransformComponent::getUp() const { return up; }

const glm::mat4x4& TransformComponent::getModelMatrix() const
{
	auto parent = gameObject->getParent();
	model = parent ? parent->getComponent<TransformComponent>()->getModelMatrix() : glm::mat4(1.f);

	model = glm::scale(model, size);
	model = glm::translate(model, position / size);
	model = glm::rotate(model, glm::radians(rotation.z), constants::worldForward);
	model = glm::rotate(model, glm::radians(rotation.y), constants::worldUp);
	model = glm::rotate(model, glm::radians(rotation.x), constants::worldRight);

	return model;
}

void TransformComponent::deserialize(const nlohmann::json& jsonObject)
{
	auto positionObj = jsonObject["position"];
	setPosition(glm::vec3(positionObj["x"],
						  positionObj["y"],
						  positionObj["z"]));

	auto rotationObj = jsonObject["rotation"];
	setRotation(glm::vec3(rotationObj["x"],
						  rotationObj["y"],
						  rotationObj["z"]));

	auto sizeObj = jsonObject["size"];
	setSize(glm::vec3(sizeObj["x"],
					  sizeObj["y"],
					  sizeObj["z"]));
}

void TransformComponent::serialize(nlohmann::json& jsonObject)
{
	nlohmann::json component;

	nlohmann::json positionJson;
	positionJson["x"] = position.x;
	positionJson["y"] = position.y;
	positionJson["z"] = position.z;

	nlohmann::json rotationJson;
	rotationJson["x"] = rotation.x;
	rotationJson["y"] = rotation.y;
	rotationJson["z"] = rotation.z;

	nlohmann::json sizeJson;
	sizeJson["x"] = size.x;
	sizeJson["y"] = size.y;
	sizeJson["z"] = size.z;

	component["position"] = positionJson;
	component["rotation"] = rotationJson;
	component["size"] = sizeJson;

	jsonObject["TransformComponent"] = component;
}