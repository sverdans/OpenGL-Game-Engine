#pragma once
#include <string>
#include <vector>
#include <map>

#include <glm/vec2.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Renderer/ShaderProgram.h"
#include "../Renderer/Texture.h"
#include "../Renderer/Material.h"
#include "../Renderer/Model.h"
#include "../Renderer/Renderer.h"

class ResourceManager
{
private:
	friend class Editor;

	static std::map<std::string, ShaderProgram*> shaders;
	static std::map<std::string, Material*> materials;
	static std::map<std::string, Texture*> textures;
	static std::map<std::string, Model*> models;

	static void processNode(std::vector<Mesh*>& meshes, aiNode* node, const aiScene* scene);
	static Mesh* processMesh(aiMesh* mesh, const aiScene* scene);

	static std::vector<Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

public:
	ResourceManager() = default;
	~ResourceManager() = default;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator = (const ResourceManager&) = delete;

	static ShaderProgram* loadShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
	static ShaderProgram* getShader(const std::string& name);
	static bool containShader(const std::string& name);

	static Texture* loadTexture(const std::string& name, const std::string& texturePath, const std::string& type);
	static Texture* getTexture(const std::string& name);
	static bool containTexture(const std::string& name);
	
	static Material* loadMaterial(const std::string& name);
	static Material* getMaterial(const std::string& name);
	static bool containMaterial(const std::string& name);

	static Model* loadModel(const std::string& name, const std::string& filepath);
	static Model* getModel(const std::string& name);
	static bool containModel(const std::string& name);

	static std::string getShaderName(const ShaderProgram* shader)
	{
		for (const auto& it : shaders)
		{
			if (it.second == shader)
				return it.first;
		}
		return "";
	}
	static std::string getModelName(const Model* model)
	{
		for (const auto& it : models)
		{
			if (it.second == model)
				return it.first;
		}
		return "";
	}

	static void loadResources(const std::string& filePath);
	static void deleteResources();
};