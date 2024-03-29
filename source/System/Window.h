#pragma once
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

class Window final
{
	friend class Editor;
public:
	static Window& Instance()
	{
		static Window instance;
		return instance;
	}

	bool Initialize(int width, int height, const std::string& sTitle);
	bool ShouldRun() const;
	void Finalize();

	void Update();
	void Resize(int width, int height);

	float GetWidth() const;
	float GetHeight() const;
	const glm::vec2& GetSize() const;

private:
	static void OnResize(GLFWwindow* window, int width, int height)
	{
		Window::Instance().Resize(width, height);
	}

private:
	Window() = default;
	~Window() = default;
	Window(const Window&) = delete;
	Window(const Window&&) = delete;
	Window& operator = (const Window&) = delete;
	Window& operator = (const Window&&) = delete;

private:
	GLFWwindow* mpWindow;
	glm::vec2 mSize;
};
