#pragma once
#include <string>
#include <vector>
#include <map>

#include <glm/vec2.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Renderer/ShaderProgram.h>
#include <Renderer/Texture.h>
#include <Renderer/Material.h>
#include <Renderer/Model.h>
#include <Renderer/Renderer.h>

class ResourceManager
{
	friend class Editor;
public:
	static ResourceManager& Instance() 
	{ 
		static ResourceManager instance;
		return instance;
	}

	ShaderProgram* LoadShader(const std::string& sName, const std::string& vertexPath, const std::string& fragmentPath);
	Texture* LoadTexture(const std::string& sName, const std::string& texturePath, const std::string& type);
	Material* LoadMaterial(const std::string& sName);
	Model* LoadModel(const std::string& sName, const std::string& filepath);
	
	ShaderProgram* GetShader(const std::string& sName);
	Texture* GetTexture(const std::string& sName);
	Material* GetMaterial(const std::string& sName);
	Model* GetModel(const std::string& sName);

	bool ContainShader(const std::string& sName) const;
	bool ContainTexture(const std::string& sName) const;
	bool ContainMaterial(const std::string& sName) const;
	bool ContainModel(const std::string& sName) const;

	std::string GetShaderName(const ShaderProgram* pShader) const;
	std::string GetModelName(const Model* pModel) const;

	void Load(const std::string& sFilePath);
	void Clear();

private:
	std::map<std::string, ShaderProgram*> mShaders;
	std::map<std::string, Material*> mMaterials;
	std::map<std::string, Texture*> mTextures;
	std::map<std::string, Model*> mModels;

	void ProcessNode(std::vector<Mesh*>& meshes, aiNode* node, const aiScene* scene);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture*> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

private:
	ResourceManager() = default;
	~ResourceManager() = default;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager(const ResourceManager&&) = delete;
	ResourceManager& operator = (const ResourceManager&) = delete;
	ResourceManager& operator = (const ResourceManager&&) = delete;
};