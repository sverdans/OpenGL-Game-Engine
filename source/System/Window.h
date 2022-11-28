#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

class Window
{
private:
	static void onResize(GLFWwindow* window, int width, int height)
	{
		size = glm::vec2(width, height);
		glViewport(0, 0, width, height);
	}

public:
	static GLFWwindow* window;
	static glm::vec2 size;

	Window() = default;
	~Window() = default;
	Window(const Window&) = delete;
	Window& operator = (const Window&) = delete;

	static void init(const glm::vec2& windowSize, const char* windowTitle);

	static void update();

	static void quit();
};
