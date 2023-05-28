#include <Game/Components/ModelRendererComponent.h>

#include <Game/GameObject.h>
#include <Game/Components/TransformComponent.h>

#include <Resources/ResourceManager.h>

#include <Renderer/ShaderProgram.h>
#include <Renderer/Model.h>
#include <Renderer/Mesh.h>

ModelRendererComponent::ModelRendererComponent(GameObject* gameObject) 
	: RendererComponent(gameObject), drawMode(Renderer::DrawMode::Triangles) {}

void ModelRendererComponent::setModel(const Model* model) 
{ 
	this->model = model;
}

void ModelRendererComponent::setShader(const ShaderProgram* shader) { this->shader = shader; }

void ModelRendererComponent::setColor(const glm::vec3& color) { this->color = color; }

void ModelRendererComponent::setSpecularStrength(const float specularStrength)
{
	this->specularStrength = specularStrength > 0.f ? (specularStrength < 1.f ? specularStrength : 1.f) : 0.f;
}

void ModelRendererComponent::setSpecularity(const int specularity)
{
	this->specularity = specularity > 0 ? (specularity < 256 ? specularity : 256) :0;
}

void ModelRendererComponent::render()
{
	shader->use();
	shader->setMatrix("modelMat", gameObject->getComponent<TransformComponent>()->getModelMatrix());
	shader->setVec3("modelColor", color);

	shader->setInt("specularity", specularity);
	shader->setFloat("specularStrength", specularStrength);

	shader->setInt("useMaterial", useMaterial);

	for (auto& mesh : model->meshes)
		mesh->render(shader);

	shader->unuse();
}

void ModelRendererComponent::deserialize(const nlohmann::json& jsonObject)
{
	shader = ResourceManager::getShader(jsonObject["shader"]);
	model = ResourceManager::getModel(jsonObject["model"]);
	
	setSpecularStrength(jsonObject["specularStrength"]);
	setSpecularity(jsonObject["specularity"]);

	color = glm::vec3(jsonObject["color"]["r"], 
					  jsonObject["color"]["g"],
					  jsonObject["color"]["b"]);

	useMaterial = jsonObject["useTexture"];
}

void ModelRendererComponent::serialize(nlohmann::json& jsonObject)
{
	nlohmann::json colorJson;
	colorJson["r"] = color.r;
	colorJson["g"] = color.g;
	colorJson["b"] = color.b;

	nlohmann::json component;
	component["shader"] = ResourceManager::getShaderName(shader);
	component["model"] = ResourceManager::getModelName(model);
	component["color"] = colorJson;

	component["useTexture"] = useMaterial;
	component["specularStrength"] = specularStrength;
	component["specularity"] = specularity;

	jsonObject[name()] = component;
}