#define _USE_MATH_DEFINES
#include "HyperboloidRendererComponent.h"

#include "TransformComponent.h"

#include "../GameObject.h"
#include "../ObjectsManager.h"

HyperboloidRendererComponent::HyperboloidRendererComponent(GameObject* gameObject) : RendererComponent(gameObject)
{	
	vertexArray = new VertexArray();
	vertexCoordsBuffer = new VertexBuffer();
	indexBuffer = new IndexBuffer();

	getHyberboloid();

	nextColor = glm::vec3(
			rand() % 255 / 255.f,
			rand() % 255 / 255.f,
			rand() % 255 / 255.f
		);
	colorDelta = nextColor - color;
}

void HyperboloidRendererComponent::getHyberboloid()
{
	float height = 8.f;
	float h = 1;
	float deltaH = (height - h) / layersCount;

	int vertexCount = (layersCount * pointsPerLayer) * 2;
	GLfloat* vertexCoords = new GLfloat[vertexCount * 3];
	glm::vec2 center = glm::vec2(0, 0);

	int layerIndex = 0;

	for (int t = 0; t < layersCount; ++t)
	{
		float R = sqrt(h * h - 1);
		for (int i = 0; i < pointsPerLayer; ++i)
		{
			int j = (layerIndex * pointsPerLayer + i) * 3;
			int k = ((layersCount + layerIndex) * pointsPerLayer + i) * 3;

			vertexCoords[j] = center.x + R * cos(2.0 * M_PI * i / (double)pointsPerLayer);
			vertexCoords[j + 1] = h;
			vertexCoords[j + 2] = center.y + R * sin(2.0 * M_PI * i / (double)pointsPerLayer);

			vertexCoords[k] = vertexCoords[j];
			vertexCoords[k + 1] = -h;
			vertexCoords[k + 2] = vertexCoords[j + 2];
		}

		layerIndex++;
		h += deltaH;
	}

	int indicesCount = ((layersCount - 1) * pointsPerLayer * 6) * 2;
	int indicesHalf = indicesCount / 2;
	int vertexHalfCount = vertexCount / 2;

	GLuint* indices = new GLuint[indicesCount];

	for (int curLayer = 0; curLayer < layersCount - 1; ++curLayer)
	{
		for (int curPoint = 0; curPoint < pointsPerLayer; ++curPoint)
		{
			int i = curLayer * pointsPerLayer + curPoint;
			int j = curPoint == pointsPerLayer - 1 ? curLayer * pointsPerLayer : i + 1;

			int nextI = (curLayer + 1) * pointsPerLayer + curPoint;
			int nextJ = (curPoint == pointsPerLayer - 1) ? (curLayer + 1) * pointsPerLayer : nextI + 1;

			int u = (curLayer * pointsPerLayer + curPoint) * 6;

			indices[u] = i;
			indices[u + 1] = j;
			indices[u + 2] = nextI;

			indices[u + 3] = nextI;
			indices[u + 4] = nextJ;
			indices[u + 5] = j;

			indices[indicesHalf + u] = vertexHalfCount + i;
			indices[indicesHalf + u + 1] = vertexHalfCount + j;
			indices[indicesHalf + u + 2] = vertexHalfCount + nextI;

			indices[indicesHalf + u + 3] = vertexHalfCount + nextI;
			indices[indicesHalf + u + 4] = vertexHalfCount + nextJ;
			indices[indicesHalf + u + 5] = vertexHalfCount + j;
		}
	}

	vertexCoordsBuffer->init(vertexCoords, vertexCount * 3 * sizeof(GLfloat));
	VertexBufferLayout vertexCoordsLayout;
	vertexCoordsLayout.addElementLayoutFloat(3, false);
	vertexArray->addBuffer(*vertexCoordsBuffer, vertexCoordsLayout);

	indexBuffer = new IndexBuffer(indices, indicesCount);

	vertexArray->unbind();
	indexBuffer->unbind();

	delete[] indices;
	delete[] vertexCoords;
}

void HyperboloidRendererComponent::setLayersCount(int layersCount)
{
	delete vertexArray;
	delete vertexCoordsBuffer;
	delete indexBuffer;

	this->layersCount = layersCount;

	vertexArray = new VertexArray();
	vertexCoordsBuffer = new VertexBuffer();
	indexBuffer = new IndexBuffer();

	getHyberboloid();
}

void HyperboloidRendererComponent::setPointsPerLayerCount(int pointsPerLayer)
{
	delete vertexArray;
	delete vertexCoordsBuffer;
	delete indexBuffer;

	this->pointsPerLayer = pointsPerLayer;

	vertexArray = new VertexArray();
	vertexCoordsBuffer = new VertexBuffer();
	indexBuffer = new IndexBuffer();

	getHyberboloid();
}

HyperboloidRendererComponent::~HyperboloidRendererComponent() {}

void HyperboloidRendererComponent::render()
{
	if (!renderEnable)
		return;

	shader->use();
	shader->setMatrix("modelMat", gameObject->getComponent<TransformComponent>()->getModelMatrix());
	shader->setVec3("color", color);
	vertexArray->bind();
	indexBuffer->bind();
	glDrawElements(GL_TRIANGLES, indexBuffer->getCount(), GL_UNSIGNED_INT, nullptr);
	shader->unuse();
}

void HyperboloidRendererComponent::update()
{
	color.x += colorDelta.x * Time::getDeltaTime() * 0.001;
	color.y += colorDelta.y * Time::getDeltaTime() * 0.001;
	color.z += colorDelta.z * Time::getDeltaTime() * 0.001;

	if ((colorDelta.x > 0 && color.x >= nextColor.x || colorDelta.x < 0 && color.x <= nextColor.x) &&
		(colorDelta.y > 0 && color.y >= nextColor.y || colorDelta.y < 0 && color.y <= nextColor.y) &&
		(colorDelta.z > 0 && color.z >= nextColor.z || colorDelta.z < 0 && color.z <= nextColor.z))
	{
		nextColor = glm::vec3(
			rand() % 255 / 255.f,
			rand() % 255 / 255.f,
			rand() % 255 / 255.f
		);
		colorDelta = nextColor - color;
	}
}

void HyperboloidRendererComponent::deserialize(const nlohmann::json& jsonObject)
{
	renderEnable = jsonObject["renderEnable"];
	shader = ResourceManager::getShader(jsonObject["Shader"]);
}
