#include "ModelRendererComponent.h"

#include "TransformComponent.h"
#include "../GameObject.h"

#include "../../Resources/ResourceManager.h"
#include "../../Renderer/ShaderProgram.h"
#include "../../Renderer/Model.h"
#include "../../Renderer/Mesh.h"

ModelRendererComponent::ModelRendererComponent(GameObject* gameObject) 
	: RendererComponent(gameObject), drawMode(Renderer::DrawMode::Triangles) {}

void ModelRendererComponent::setModel(const Model* model) 
{ 
	this->model = model;
	
	if (!model->haveTexture)
		renderWithTexture = false;
}

void ModelRendererComponent::setShader(const ShaderProgram* shader) { this->shader = shader; }

void ModelRendererComponent::render()
{
	shader->use();
	shader->setMatrix("modelMat", gameObject->getComponent<TransformComponent>()->getModelMatrix());
	shader->setVec3("modelColor", color);

	for (auto& mesh : model->meshes)
		mesh->render();

	shader->unuse();
}

void ModelRendererComponent::deserialize(const nlohmann::json& jsonObject)
{
	shader = ResourceManager::getShader(jsonObject["shader"]);
	model = ResourceManager::getModel(jsonObject["model"]);

	color = glm::vec3(jsonObject["color"]["r"], 
					  jsonObject["color"]["g"],
					  jsonObject["color"]["b"]);
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

	jsonObject["ModelRenderer"] = component;
}