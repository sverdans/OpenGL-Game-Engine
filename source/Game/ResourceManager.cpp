#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include <nlohmann/json.hpp>

#include <stb_image.h>

#include <General/Utils.h>
#include <Renderer/Mesh.h>
#include <Renderer/Material.h>
#include <Game/ResourceManager.h>

//--Load--------------------------------------------------------------------------------------------------------

ShaderProgram* ResourceManager::LoadShader(
	const std::string& sShaderName,
	const std::string& sVertexPath,
	const std::string& sFragmentPath)
{
	std::string sVertexString = utils::FetchDataFromFile(sVertexPath);
	
	if (sVertexString.empty())
	{
		std::cout << "Error! File with vertex shader is empty. File path: " << sVertexPath << "." << std::endl;
		exit(-1);
	}

	std::string sFragmentString = utils::FetchDataFromFile(sFragmentPath);
	if (sFragmentString.empty())
	{
		std::cout << "Error! File with fragment shader is empty. File path: " << sFragmentPath << "." << std::endl;
		exit(-1);
	}

	ShaderProgram* pProgram = new ShaderProgram(sVertexString, sFragmentString);
	
	if (!pProgram->isCompiled())
	{
		std::cout << "Error! Shader is not compiled. Shader name: " << sShaderName << "." << std::endl;
		exit(-1);
	}

	std::cout << "Shader '" << sShaderName << "'created" << std::endl;
	mShaders.emplace(sShaderName, pProgram);
	return pProgram;
}

Texture* ResourceManager::LoadTexture(
	const std::string& sTextureName,
	const std::string& sTexturePath,
	const std::string& sType)
{
	int channels = 0;
	int width = 0;
	int height = 0;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(sTexturePath.c_str(), &width, &height, &channels, 0);

	if (!pixels)
	{
		std::cout << "Error! Texture texture was not loaded. Texture name: " << sTextureName << "." << std::endl;
		exit(-1);
	}

	Texture* texture = new Texture(width, height, pixels, channels, GL_NEAREST, GL_CLAMP_TO_EDGE);
	stbi_image_free(pixels);

	texture->type = sType;

	std::cout << "Texture '" << sTextureName << "'created" << std::endl;
	mTextures.emplace(std::make_pair(sTextureName, texture));
	return texture;
}

Material* ResourceManager::LoadMaterial(const std::string& sName)
{
	Material* pMaterial = new Material();

	std::cout << "Material '" << sName << "'created" << std::endl;
	mMaterials.emplace(std::make_pair(sName, pMaterial));
	return pMaterial;
}

Model* ResourceManager::LoadModel(const std::string& sName, const std::string& sFilePath)
{
	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(sFilePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	
	if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return nullptr;
	}

	std::vector<Mesh*> meshes;
	ProcessNode(meshes, pScene->mRootNode, pScene);

	Model* pModel = new Model(meshes, Renderer::EnDrawMode::Triangles);

	std::cout << "Model '" << sName << "'created" << std::endl;
	mModels.emplace(std::make_pair(sName, pModel));
	return pModel;
}

//--Get---------------------------------------------------------------------------------------------------------

ShaderProgram* ResourceManager::GetShader(const std::string& sName)
{
	auto it = mShaders.find(sName);
	if (it == mShaders.end())
	{
		std::cout << "Warning! Shader not found. Shader name: " << sName << "." << std::endl;
		return nullptr;
	}
	return it->second;
}

Texture* ResourceManager::GetTexture(const std::string& sName)
{
	auto it = mTextures.find(sName);
	if (it == mTextures.end())
	{
		std::cout << "Warning! Texture not found. Texture name: " << sName << "." << std::endl;
		return nullptr;
	}
	return it->second;
}

Material* ResourceManager::GetMaterial(const std::string& sName)
{
	auto it = mMaterials.find(sName);
	if (it == mMaterials.end())
	{
		std::cout << "Warning! Material not found. Material name: " << sName << "." << std::endl;
		return nullptr;
	}
	return it->second;
}

Model* ResourceManager::GetModel(const std::string& sName)
{
	auto it = mModels.find(sName);
	if (it == mModels.end())
	{
		std::cout << "Warning! Model not found. Model name: " << sName << "." << std::endl;
		return nullptr;
	}
	return it->second;
}

//--Contain-----------------------------------------------------------------------------------------------------

bool ResourceManager::ContainShader(const std::string& sName) const
{
	return !(mShaders.find(sName) == mShaders.end());
}

bool ResourceManager::ContainTexture(const std::string& sName) const
{
	return !(mTextures.find(sName) == mTextures.end());
}

bool ResourceManager::ContainMaterial(const std::string& sName) const
{
	return !(mMaterials.find(sName) == mMaterials.end());
}

bool ResourceManager::ContainModel(const std::string& sName) const
{
	return !(mModels.find(sName) == mModels.end());
}

//--GetName-----------------------------------------------------------------------------------------------------

std::string ResourceManager::GetShaderName(const ShaderProgram* pShader) const
{
	for (const auto& [sShaderName, pIt] : mShaders)
	{
		if (pIt == pShader)
			return sShaderName;
	}
	return "";
}

std::string ResourceManager::GetModelName(const Model* pModel) const
{
	for (const auto& [sModelName, pIt] : mModels)
	{
		if (pIt == pModel)
			return sModelName;
	}
	return "";
}

//--------------------------------------------------------------------------------------------------------------

void ResourceManager::Load(const std::string& sFilePath)
{
	json document;
	utils::ParseJsonFile(sFilePath, document);
	const auto& jsonResources = document["resources"];
	
	if (jsonResources.contains("Shaders"))
	{
		auto jsonShaders = jsonResources["Shaders"];
		if (jsonShaders.is_array())
		{
			for (const auto& currentShader : jsonShaders)
			{
				const std::string name = currentShader["name"];
				const std::string vertexPath = currentShader["vertexPath"];
				const std::string fragmentPath = currentShader["fragmentPath"];
				LoadShader(name, vertexPath, fragmentPath);
			}
		}
	}

	if (jsonResources.contains("Textures"))
	{
		auto jsonTextures = jsonResources["Textures"];
		if (jsonTextures.is_array())
		{
			for (const auto& currentTexture : jsonTextures)
			{
				const std::string name = currentTexture["name"];
				const std::string path = currentTexture["path"];
				std::vector<std::pair<std::string, std::pair<glm::vec2, glm::vec2>>> subTextures;
			}
		}
	}

	if (jsonResources.contains("Models"))
	{
		auto jsonModels = jsonResources["Models"];
		if (jsonModels.is_array())
		{
			for (const auto& currentModel : jsonModels)
			{
				const std::string name = currentModel["name"];
				const std::string modelFile = currentModel["filepath"];
				LoadModel(name, modelFile);
			}
		}
	}
}

void ResourceManager::Clear()
{
	for (auto& [sShaderName, pShader] : mShaders)
		delete pShader;
	mShaders.clear();

	for (auto& [sTextureName, pTexture] : mTextures)
		delete pTexture;
	mTextures.clear();
	
	for (auto& [sModelName, pModel] : mModels)
		delete pModel;
	mModels.clear();
}

void ResourceManager::ProcessNode(std::vector<Mesh*>& meshes, aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(meshes, node->mChildren[i], scene);
	}
}

Mesh* ResourceManager::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		glm::vec3 vector;

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		if (mesh->HasNormals())
		{
			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;
		}

		if (mesh->mTextureCoords[0])
		{
			vertex.texturePosition.x = mesh->mTextureCoords[0][i].x;
			vertex.texturePosition.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.texturePosition = glm::vec2(0.0f, 0.0f);
		}
		
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	Material* material = nullptr;

	aiMaterial* sourceMaterial = scene->mMaterials[mesh->mMaterialIndex];
	std::string materialName(sourceMaterial->GetName().C_Str());

	if (ContainMaterial(materialName))
	{
		material = GetMaterial(materialName);
	}
	else
	{
		material = LoadMaterial(materialName);
		
		aiColor3D color(0.f, 0.f, 0.f);
		sourceMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material->color.r = color.r;
		material->color.g = color.g;
		material->color.b = color.b;

		auto diffuseMaps = LoadMaterialTextures(sourceMaterial, aiTextureType_DIFFUSE, "diffuseTexture");
		material->textures.insert(material->textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		auto specularMaps = LoadMaterialTextures(sourceMaterial, aiTextureType_SPECULAR, "specularTexture");
		material->textures.insert(material->textures.end(), specularMaps.begin(), specularMaps.end());
		auto normalMaps = LoadMaterialTextures(sourceMaterial, aiTextureType_HEIGHT, "normalTexture");
		material->textures.insert(material->textures.end(), normalMaps.begin(), normalMaps.end());
		auto heightMaps = LoadMaterialTextures(sourceMaterial, aiTextureType_AMBIENT, "heightTexture");
		material->textures.insert(material->textures.end(), heightMaps.begin(), heightMaps.end());
	}

	return new Mesh(vertices, indices, material, !mesh->HasNormals());
}

std::vector<Texture*> ResourceManager::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture*> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		
		std::string texturePath = str.C_Str();

		if (ContainTexture(str.C_Str()))
			textures.push_back(GetTexture(str.C_Str()));
		else
			textures.push_back(LoadTexture(str.C_Str(), texturePath, typeName));
	}
	return textures;
}
