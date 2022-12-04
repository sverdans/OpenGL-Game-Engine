#pragma once
#include <glad/glad.h>
#include <glm/vec3.hpp>
#include <nlohmann/json.hpp>

struct GLFWwindow;
class VertexArray;
class IndexBuffer;

class Renderer 
{
public:
	enum class PolygonMode
	{
		Fill,
		Line,
		Point
	};

private:
	static int pointSize;
	static glm::vec3 clearColor;
	static PolygonMode polygonMode;
	static bool depthTest;

	static constexpr char* polygonModeStrings[] {
		"Fill",
		"Line",
		"Point"
	};


public:
	enum class DrawMode
	{
		Triangles = GL_TRIANGLES,
		TriangleFan = GL_TRIANGLE_FAN,
		Line = GL_LINES,
		LineStrip = GL_LINE_STRIP,
		LineLoop = GL_LINE_LOOP
	};

	static bool init(GLFWwindow* pWindow);

	static void draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const DrawMode mode);
	static void setClearColor(const float r, const float g, const float b, const float a);
	static void clear();
	static void setViewport(const unsigned int width, const unsigned int height, const unsigned int left_offset = 0, const unsigned int bottom_offset = 0);
	static void enableDepthTest();
	static void disableDepthTest();

	static void setPointSize(const int size);
	static void setPolygonMode(const unsigned int polygonMode);

	static const char* getVendorString();
	static const char* getRendererString();
	static const char* getVersionString();

	static void drawTools();
};