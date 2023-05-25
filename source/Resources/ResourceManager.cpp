#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include <nlohmann/json.hpp>

#include "../../external/stb_image.h"

#include "ResourceManager.h"
#include "../System/Parser.h"
#include "../Renderer/Mesh.h"
#include "../Renderer/Material.h"

ShaderProgram* ResourceManager::loadShader(const std::string& programName,
										   const std::string& vertexPath,
										   const std::string& fragmentPath)
{
	std::string vertexString = Parser::getFileString(vertexPath);
	
	if (vertexString.empty())
	{
		std::cout << "Error! File with vertex shader is empty. File path: " << vertexPath << "." << std::endl;
		exit(-1);
	}

	std::string fragmentString = Parser::getFileString(fragmentPath);
	if (fragmentString.empty())
	{
		std::cout << "Error! File with fragment shader is empty. File path: " << vertexPath << "." << std::endl;
		exit(-1);
	}

	ShaderProgram* program = new ShaderProgram(vertexString, fragmentString);
	
	if (!program->isCompiled())
	{
		std::cout << "Error! Shader is not compiled. Shader name: " << programName << "." << std::endl;
		exit(-1);
	}

	shaders.emplace(programName, program);
	return program;
}

ShaderProgram* ResourceManager::getShader(const std::string& name)
{
	auto it = shaders.find(name);
	if (it == shaders.end())
	{
		std::cout << "Warning! Shader not found. Shader name: " << name << "." << std::endl;
		return nullptr;
	}
	return it->second;
}

bool ResourceManager::containShader(const std::string& name)
{
	return !(shaders.find(name) == shaders.end());
}



Texture* ResourceManager::loadTexture(const std::string& name,
									  const std::string& texturePath,
									  const std::string& type)
{
	int channels = 0;
	int width = 0;
	int height = 0;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(texturePath.c_str(), &width, &height, &channels, 0);

	if (!pixels)
	{
		std::cout << "Error! Texture texture was not loaded. Texture name: " << name << "." << std::endl;
		exit(-1);
	}

	Texture* texture = new Texture(width, height, pixels, channels, GL_NEAREST, GL_CLAMP_TO_EDGE);
	texture->type = type;

	textures.emplace(std::make_pair(name, texture));

	stbi_image_free(pixels);

	return texture;
}

Texture* ResourceManager::getTexture(const std::string& name)
{
	auto it = textures.find(name);
	if (it == textures.end())
	{
		std::cout << "Warning! Texture not found. Texture name: " << name << "." << std::endl;
		return nullptr;
	}
	return it->second;
}

bool ResourceManager::containTexture(const std::string& name)
{
	return !(textures.find(name) == textures.end());
}



Material* ResourceManager::loadMaterial(const std::string& name)
{
	Material* material = new Material();
	materials.emplace(std::make_pair(name, material));
	return material;
}

Material* ResourceManager::getMaterial(const std::string& name)
{
	auto it = materials.find(name);
	if (it == materials.end())
	{
		std::cout << "Warning! Material not found. Material name: " << name << "." << std::endl;
		return nullptr;
	}
	return it->second;
}

bool ResourceManager::containMaterial(const std::string& name)
{
	return !(materials.find(name) == materials.end());
}



Model* ResourceManager::loadModel(const std::string& name, const std::string& filepath)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filepath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return nullptr;
	}

	std::vector<Mesh*> meshes;
	processNode(meshes, scene->mRootNode, scene);
	Model* model = new Model(meshes, Renderer::DrawMode::Triangles);
	
	models.emplace(std::make_pair(name, model));
	return model;
}

Model* ResourceManager::getModel(const std::string& name)
{
	auto it = models.find(name);
	if (it == models.end())
	{
		std::cout << "Warning! Model not found. Model name: " << name << "." << std::endl;
		return nullptr;
	}
	return it->second;
}

bool ResourceManager::containModel(const std::string& name)
{
	return !(models.find(name) == models.end());
}



void ResourceManager::loadResources(const std::string& filePath)
{
	nlohmann::json document;
	Parser::parseJsonFile(filePath, document);

	if (document.contains("Shaders"))
	{
		auto jsonShaders = document["Shaders"];
		if (jsonShaders.is_array())
		{
			for (const auto& currentShader : jsonShaders)
			{
				const std::string name = currentShader["name"];
				const std::string vertexPath = currentShader["vertexPath"];
				const std::string fragmentPath = currentShader["fragmentPath"];
				loadShader(name, Parser::getDirectoryPath() + vertexPath, Parser::getDirectoryPath() + fragmentPath);
			}
		}
	}

	if (document.contains("Textures"))
	{
		auto jsonTextures = document["Textures"];
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

	if (document.contains("Models"))
	{
		auto jsonModels = document["Models"];
		if (jsonModels.is_array())
		{
			for (const auto& currentModel : jsonModels)
			{
				const std::string name = currentModel["name"];
				const std::string modelFile = currentModel["filepath"];
				loadModel(name, Parser::getDirectoryPath() + modelFile);
			}
		}
	}
}

void ResourceManager::deleteResources()
{
	for (auto it = shaders.cbegin(); it != shaders.cend(); ++it)
		delete it->second;
	shaders.clear();

	for (auto it = textures.cbegin(); it != textures.cend(); ++it)
		delete it->second;
	textures.clear();

	for (auto it = models.cbegin(); it != models.cend(); ++it)
		delete it->second;
	models.clear();
}



void ResourceManager::processNode(std::vector<Mesh*>& meshes, aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(meshes, node->mChildren[i], scene);
	}
}

Mesh* ResourceManager::processMesh(aiMesh* mesh, const aiScene* scene)
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

	if (ResourceManager::containMaterial(materialName))
	{
		material = ResourceManager::getMaterial(materialName);
	}
	else
	{
		material = ResourceManager::loadMaterial(materialName);
		
		aiColor3D color(0.f, 0.f, 0.f);
		sourceMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		material->color.r = color.r;
		material->color.g = color.g;
		material->color.b = color.b;

		auto diffuseMaps = loadMaterialTextures(sourceMaterial, aiTextureType_DIFFUSE, "diffuseTexture");
		material->textures.insert(material->textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		auto specularMaps = loadMaterialTextures(sourceMaterial, aiTextureType_SPECULAR, "specularTexture");
		material->textures.insert(material->textures.end(), specularMaps.begin(), specularMaps.end());
		auto normalMaps = loadMaterialTextures(sourceMaterial, aiTextureType_HEIGHT, "normalTexture");
		material->textures.insert(material->textures.end(), normalMaps.begin(), normalMaps.end());
		auto heightMaps = loadMaterialTextures(sourceMaterial, aiTextureType_AMBIENT, "heightTexture");
		material->textures.insert(material->textures.end(), heightMaps.begin(), heightMaps.end());
	}

	return new Mesh(vertices, indices, material, !mesh->HasNormals());
}

std::vector<Texture*> ResourceManager::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture*> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		
		std::string texturePath = Parser::getDirectoryPath() + "resources\\models\\" + str.C_Str();

		if (ResourceManager::containTexture(str.C_Str()))
			textures.push_back(ResourceManager::getTexture(str.C_Str()));
		else
			textures.push_back(ResourceManager::loadTexture(str.C_Str(), texturePath, typeName));
	}
	return textures;
}

std::map<std::string, ShaderProgram*> ResourceManager::shaders;
std::map<std::string, Material*> ResourceManager::materials;
std::map<std::string, Texture*> ResourceManager::textures;
std::map<std::string, Model*> ResourceManager::models;