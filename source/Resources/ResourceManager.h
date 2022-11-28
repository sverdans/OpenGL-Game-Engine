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
#include "../Renderer/Sprite.h"
#include "../Renderer/Model.h"
#include "../Renderer/Renderer.h"

class ResourceManager
{
private:
	friend class Editor;

	static std::map<std::string, ShaderProgram*> shaders;
	static std::map<std::string, Model*> models;
	static std::map<std::string, Texture*> textures;
	static std::map<std::string, Sprite*> sprites;

	static void processNode(std::vector<Mesh*>& meshes, aiNode* node, const aiScene* scene);
	static Mesh* processMesh(aiMesh* mesh, const aiScene* scene);

public:
	ResourceManager() = default;
	~ResourceManager() = default;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator = (const ResourceManager&) = delete;

	static ShaderProgram* loadShader(const std::string& name,
									 const std::string& vertexPath,
									 const std::string& fragmentPath);
	static ShaderProgram* getShader(const std::string& name);

	static Texture* loadTexture(const std::string& textureName,
								const std::string& texturePath,
								const std::vector<std::pair<std::string, std::pair<glm::vec2, glm::vec2>>>& subTextureNames);
	static Texture* getTexture(const std::string& name);
	
	static Sprite* loadSprite(const std::string& spriteName, const std::string& textureName);
	static Sprite* getSprite(const std::string& name);

	static Model* loadModel(const std::string& name, const std::string& filepath);
	static Model* addModel(const std::string& name, const std::vector<Mesh*> meshes, const Renderer::DrawMode drawMode);
	static Model* getModel(const std::string& name);

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
	static void deleteAllResources();
};