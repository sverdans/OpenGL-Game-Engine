#include <System/Window.h>
#include <System/InputHandler.h>
#include <Editor/UiHandler.h>
#include <Renderer/Renderer.h>

bool Window::Initialize(const glm::vec2& size, const char* title)
{
	if (!glfwInit())
	{
		std::cout << "Error! GLFW initialization failed." << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	mSize = size;
	mpWindow = glfwCreateWindow(size.x, size.y, title, nullptr, nullptr);

	if (!mpWindow)
	{
		glfwTerminate();
		std::cout << "Error! GLFW window creating failed." << std::endl;
		return false;
	}

	glfwMakeContextCurrent(mpWindow);

	if (!Renderer::init(mpWindow))
	{
		std::cout << "Error! GLAD initialization failed." << std::endl;
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
