#pragma once
#include <vector>
#include <string>

#include "Renderer.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Mesh;

class Model
{
private:
	friend class ResourceManager;
	friend class ModelRendererComponent;

	mutable Renderer::DrawMode drawMode;
	std::vector<Mesh*> meshes;

	Model(const std::vector<Mesh*>& meshes, const Renderer::DrawMode drawMode);
	~Model();

public:

	Model() = delete;
	Model(const Model&) = delete;
	Model operator = (const Model&) = delete;

	void setDrawMode(Renderer::DrawMode mode) const;
};