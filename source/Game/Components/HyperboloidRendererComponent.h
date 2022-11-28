#include "RendererComponent.h"
#include "../../Resources/ResourceManager.h"

class HyperboloidRendererComponent : public RendererComponent
{
private:
	ShaderProgram* shader = nullptr;
	VertexArray* vertexArray;
	VertexBuffer* vertexCoordsBuffer;
	IndexBuffer* indexBuffer;

	glm::vec3 color = glm::vec3(1.f, 1.f, 0);
	glm::vec3 nextColor;
	glm::vec3 colorDelta;

	int layersCount = 5;
	int pointsPerLayer = 5;

	void getHyberboloid();
public:

	HyperboloidRendererComponent() = delete;
	HyperboloidRendererComponent(const HyperboloidRendererComponent&) = delete;
	HyperboloidRendererComponent& operator = (const HyperboloidRendererComponent&) = delete;

	HyperboloidRendererComponent(GameObject* gameObject);
	~HyperboloidRendererComponent();

	void render() override;
	void update() override;

	void setLayersCount(int layersCount);
	void setPointsPerLayerCount(int pointsPerLayer);

	void deserialize(const nlohmann::json& jsonObject) override;
	void serialize(nlohmann::json& jsonObject) override {};
};