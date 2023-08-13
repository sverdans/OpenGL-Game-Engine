#include "Model.h"

#include "Mesh.h"
#include "Texture.h"

Model::~Model()
{
	for (auto& mesh : meshes)
		delete mesh;
}

Model::Model(const std::vector<Mesh*>& meshes, const Renderer::EnDrawMode eDrawMode) 
	: meshes(meshes)
	, drawMode(eDrawMode) 
{
	for (auto& mesh : this->meshes)
		mesh->setDrawMode(drawMode);
}

void Model::setDrawMode(Renderer::EnDrawMode eMode) const
{
	if (drawMode == eMode)
		return;

	drawMode = eMode;
	for (auto& mesh : meshes)
		mesh->setDrawMode(eMode);
}