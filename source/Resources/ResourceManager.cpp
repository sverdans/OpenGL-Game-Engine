#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include <nlohmann/json.hpp>

#include "../../external/stb_image.h"

#include "ResourceManager.h"
#include "../System/Parser.h"
#include "../Renderer/Mesh.h"

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

void ResourceManager::deleteAllResources()
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
	std::vector<Texture*> textures;

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

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	auto diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuseTexture");
	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	auto specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specularTexture");
	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	auto normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "normalTexture");
	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	auto heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "heightTexture");
	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

	return new Mesh(vertices, indices, textures, !mesh->HasNormals());
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


/*
unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
*/

std::map<std::string, ShaderProgram*> ResourceManager::shaders;
std::map<std::string, Texture*> ResourceManager::textures;
std::map<std::string, Model*> ResourceManager::models;