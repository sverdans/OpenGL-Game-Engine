#include "Model.h"

#include "Mesh.h"
#include "Texture.h"

Model::~Model()
{
	for (auto& mesh : meshes)
		delete mesh;
}

Model::Model(const std::vector<Mesh*>& meshes, const Renderer::DrawMode drawMode) : meshes(meshes), drawMode(drawMode) 
{
	for (auto& mesh : this->meshes)
		mesh->setDrawMode(drawMode);
}

void Model::setDrawMode(Renderer::DrawMode mode) const
{
	if (drawMode == mode)
		return;

	drawMode = mode;
	for (auto& mesh : meshes)
		mesh->setDrawMode(mode);
}