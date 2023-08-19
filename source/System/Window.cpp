#include <System/Window.h>
#include <System/InputHandler.h>
#include <Renderer/Renderer.h>

#include <spdlog/logger.h>

bool Window::Initialize(int width, int height, const std::string& sTitle)
{
	spdlog::get("main")->info("Window initialize");

	if (!glfwInit())
	{
		spdlog::get("main")->critical("GLFW initialization failed");
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	mSize = { width, height };
	mpWindow = glfwCreateWindow(mSize.x, mSize.y, sTitle.c_str(), nullptr, nullptr);

	if (!mpWindow)
	{
		glfwTerminate();
		spdlog::get("main")->critical("GLFW window creating failed");
		return false;
	}

	glfwMakeContextCurrent(mpWindow);

	if (!Renderer::init(mpWindow))
	{
		glfwTerminate();
		spdlog::get("main")->critical("GLAD initialization failed");
		return false;
	}

	glfwSetKeyCallback(mpWindow, InputHandler::setKey);
	glfwSetCursorPosCallback(mpWindow, InputHandler::setMouseCoord);
	glfwSetMouseButtonCallback(mpWindow, InputHandler::setMouseButton);
	glfwSetWindowSizeCallback(mpWindow, OnResize);
	glfwSetWindowSizeLimits(mpWindow, 800, 600, GLFW_DONT_CARE, GLFW_DONT_CARE);

	return true;
}

void Window::Finalize()
{
	spdlog::get("main")->info("Window finalize");

	if (mpWindow)
		glfwDestroyWindow(mpWindow);
	glfwTerminate();
}

bool Window::NeedFinalize() const
{
	return glfwWindowShouldClose(mpWindow);
}

void Window::Resize(int width, int height)
{
	mSize = glm::vec2(width, height);
	glViewport(0, 0, width, height);
}

void Window::Update()
{
	glfwSwapBuffers(mpWindow);
}

float Window::GetWidth() const  { return mSize.x; }
float Window::GetHeight() const { return mSize.y; }
const glm::vec2& Window::GetSize() const { return mSize; }
GLFWwindow* Window::GetWindowPtr() { return mpWindow; }
