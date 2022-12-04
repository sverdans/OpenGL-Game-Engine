#include "Renderer.h"

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include "VertexArray.h"
#include "IndexBuffer.h"

bool Renderer::init(GLFWwindow* pWindow)
{
	glfwMakeContextCurrent(pWindow);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(-1);
	}

	std::cout << "OpenGL context initialized:" << std::endl;
	std::cout << "\tVendor: " << getVendorString() << std::endl;
	std::cout << "\tRenderer: " << getRendererString() << std::endl;
	std::cout << "\tVersion: " << getVersionString() << std::endl;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

void Renderer::draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const DrawMode mode)
{
	vertexArray.bind();
	indexBuffer.bind();
	glDrawElements(static_cast<GLenum>(mode), indexBuffer.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::setClearColor(const float r, const float g, const float b, const float a)
{
	glClearColor(r, g, b, a);
}

void Renderer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setViewport(const unsigned int width, const unsigned int height, const unsigned int leftOffset, const unsigned int bottomOffset)
{
	glViewport(leftOffset, bottomOffset, width, height);
}

void Renderer::enableDepthTest()
{
	depthTest = true;
	glEnable(GL_DEPTH_TEST);
}

void Renderer::disableDepthTest()
{
	depthTest = false;
	glDisable(GL_DEPTH_TEST);
}

void Renderer::setPointSize(const int size) { glPointSize(size); }

void Renderer::setPolygonMode(const unsigned int polygonMode) { glPolygonMode(GL_FRONT_AND_BACK, polygonMode); }

const char* Renderer::getVendorString()
{
	return reinterpret_cast<const char*>(glGetString(GL_VENDOR));
}

const char* Renderer::getRendererString()
{
	return reinterpret_cast<const char*>(glGetString(GL_RENDERER));
}

const char* Renderer::getVersionString()
{
	return reinterpret_cast<const char*>(glGetString(GL_VERSION));
}

void Renderer::drawTools()
{
	int curPolygonMode = static_cast<int>(polygonMode);

	if (ImGui::Combo("PolygonMode", &curPolygonMode, polygonModeStrings, 3))
	{
		switch (curPolygonMode)
		{
			case 0: setPolygonMode(GL_FILL); break;
			case 1: setPolygonMode(GL_LINE); break;
			case 2: setPolygonMode(GL_POINT); break;
		}
	}

	if (ImGui::SliderInt("PointSize", &pointSize, 1, 10))
	{
		setPointSize(pointSize);
	}

	if (ImGui::Checkbox("DepthTest", &depthTest))
	{
		if (depthTest)
			Renderer::enableDepthTest();
		else
			Renderer::disableDepthTest();
	}
}

int Renderer::pointSize = 1;
bool Renderer::depthTest = true;
glm::vec3 Renderer::clearColor = glm::vec3(0.f, 0.f, 0.f);
Renderer::PolygonMode Renderer::polygonMode = Renderer::PolygonMode::Fill;

