#ifndef IMGUI_DEFINE_MATH_OPERATORS
	#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <System/Window.h>
#include <System/InputHandler.h>
#include <Editor/UiHandler.h>
#include <Renderer/Renderer.h>


void Window::init(const glm::vec2& windowSize, const char* windowTitle)
{
	if (!glfwInit())
	{
		std::cout << "Error! GLFW initialization failed." << std::endl;
		exit(-1);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	size = windowSize;
	window = glfwCreateWindow(size.x, size.y, windowTitle, nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		std::cout << "Error! GLFW window creating failed." << std::endl;
		exit(-1);
	}

	glfwMakeContextCurrent(window);

	if (!Renderer::init(window))
	{
		std::cout << "Error! GLAD initialization failed." << std::endl;
		exit(-1);
	}

	glfwSetKeyCallback(Window::window, InputHandler::setKey);
	glfwSetCursorPosCallback(Window::window, InputHandler::setMouseCoord);
	glfwSetMouseButtonCallback(Window::window, InputHandler::setMouseButton);
	glfwSetWindowSizeCallback(Window::window, Window::onResize);
	glfwSetWindowSizeLimits(window, 800, 600, GLFW_DONT_CARE, GLFW_DONT_CARE);

	UiHandler::onWindowCreate();
}

void Window::quit()
{
	UiHandler::onWindowClose();
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::update()
{
	glfwSwapBuffers(window);
}

GLFWwindow* Window::window;
glm::vec2 Window::size;