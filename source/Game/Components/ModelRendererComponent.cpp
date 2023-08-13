#include <Game/Components/ModelRendererComponent.h>

#include <Game/GameObject.h>
#include <Game/Components/TransformComponent.h>

#include <Resources/ResourceManager.h>

#include <Renderer/ShaderProgram.h>
#include <Renderer/Model.h>
#include <Renderer/Mesh.h>

ModelRendererComponent::ModelRendererComponent(GameObject* gameObject) 
	: RendererComponent(gameObject)
	, mpModel(nullptr)
	, mpShader(nullptr)
	, mlUseMaterial(true)
	, meDrawMode(Renderer::DrawMode::Triangles)
{ }

void ModelRendererComponent::SetModel(const Model* pModel) 
{ 
	mpModel = pModel;
}

void ModelRendererComponent::SetShader(const ShaderProgram* pShader) 
{
	mpShader = pShader;
}

void ModelRendererComponent::SetColor(const glm::vec3& color) 
{
	mColor = color;
}

void ModelRendererComponent::SetSpecularStrength(const float specularStrength)
{
	mSpecularStrength = specularStrength > 0.f 
		? (specularStrength < 1.f ? specularStrength : 1.f) 
		: 0.f;
}

void ModelRendererComponent::SetSpecularity(const int specularity)
{
	mSpecularity = specularity > 0 
		? (specularity < 256 ? specularity : 256) 
		: 0;
}

void ModelRendererComponent::Render()
{
	mpShader->use();
	mpShader->setMatrix("modelMat", GetGameObject()->GetComponent<TransformComponent>()->GetModelMatrix());
	mpShader->setVec3("modelColor", mColor);

	mpShader->setInt("specularity", mSpecularity);
	mpShader->setFloat("specularStrength", mSpecularStrength);

	mpShader->setInt("useMaterial", mlUseMaterial);

	for (auto& mesh : mpModel->meshes)
		mesh->render(mpShader);

	mpShader->unuse();
}

void ModelRendererComponent::Deserialize(const nlohmann::json& jsonObject)
{
	mpShader = ResourceManager::Instance().GetShader(jsonObject["shader"]);
	mpModel = ResourceManager::Instance().GetModel(jsonObject["model"]);
	
	SetSpecularStrength(jsonObject["specularStrength"]);
	SetSpecularity(jsonObject["specularity"]);

	mColor = {
		jsonObject["color"]["r"], 
		jsonObject["color"]["g"],
		jsonObject["color"]["b"]
	};

	mlUseMaterial = jsonObject["useTexture"];
}

void ModelRendererComponent::Serialize(nlohmann::json& jsonObject)
{
	nlohmann::json colorJson {
		{ "r", mColor.r },
		{ "g", mColor.g },
		{ "b", mColor.b },
	};

	jsonObject[Name()] = {
		{ "shader", ResourceManager::Instance().GetShaderName(mpShader) },
		{ "model" , ResourceManager::Instance().GetModelName(mpModel) },
		{ "specularStrength", mSpecularStrength },
		{ "useTexture", mlUseMaterial },
		{ "specularity", mSpecularity },
		{ "color", colorJson },
	};
}