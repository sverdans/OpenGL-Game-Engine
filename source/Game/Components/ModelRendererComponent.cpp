#include <Game/Components/ModelRendererComponent.h>

#include <Game/GameObject.h>
#include <Game/Components/TransformComponent.h>

#include <Game/ResourceManager.h>

#include <Renderer/ShaderProgram.h>
#include <Renderer/Model.h>
#include <Renderer/Mesh.h>

DECLARE_COMPONENT(ModelRendererComponent)

ModelRendererComponent::ModelRendererComponent(GameObject* gameObject) 
	: RendererComponent(gameObject)
	, mpModel(nullptr)
	, mpShader(nullptr)
	, mlUseMaterial(true)
	, meDrawMode(Renderer::EnDrawMode::Triangles)
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

Component* ModelRendererComponent::Clone() { return new ModelRendererComponent(nullptr); }
std::string ModelRendererComponent::Name() { return "ModelRendererComponent"; }

json ModelRendererComponent::Serialize()
{
	json colorJson {
		{ "r", mColor.r },
		{ "g", mColor.g },
		{ "b", mColor.b },
	};

	return {
		{ "shader", ResourceManager::Instance().GetShaderName(mpShader) },
		{ "model" , ResourceManager::Instance().GetModelName(mpModel) },
		{ "specularStrength", mSpecularStrength },
		{ "useTexture", mlUseMaterial },
		{ "specularity", mSpecularity },
		{ "color", colorJson },
	};
}

void ModelRendererComponent::Deserialize(const json& jsonObject)
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