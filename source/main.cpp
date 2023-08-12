#include <iostream>
#include <typeinfo>
#include <vector>
#include <map>

#include <System/Window.h>
#include <System/Time.h>
#include <System/InputHandler.h>

#include <Renderer/Renderer.h>
#include <Resources/ResourceManager.h>

#include <Game/ObjectsManager.h>
#include <Game/Systems/RendererSystem.h>
#include <Game/Systems/BehaviourSystem.h>

#include <Editor/Editor.h>

int main(int argc, char** argv)
{
	Window::init(glm::vec2(1000, 800), "OpenGL-Courswork");
	UiHandler::onWindowCreate();

	Renderer::setClearColor(60.f / 255.f , 60.f / 255.f, 60.f / 255.f, 0.f);

//	Renderer::setClearColor(0.f, 0.f, 0.f, 0.f);
	Renderer::enableDepthTest();

	InputHandler::init();

	Editor::init();

	LightingSystem::setAmbientColor(glm::vec3(1.f, 1.f, 1.f));
	LightingSystem::setAmbientStrength(0.225f);

	Time::setUpdateFrequency(60.0);
	Time::start();

	while (!glfwWindowShouldClose(Window::window))
	{
		glfwPollEvents();

		Time::update();

		if (Time::CheckFPS())
		{
			Renderer::clear();
			
			const auto& buttons = InputHandler::getKeys();
			if (buttons[GLFW_KEY_LEFT_CONTROL] && buttons[GLFW_KEY_Q])
				glfwSetWindowShouldClose(Window::window, GLFW_TRUE);

			BehaviourSystem::update();
			BehaviourSystem::lateUpdate();
			BehaviourSystem::onPreRender();

			LightingSystem::update();
			RendererSystem::Render();

			Editor::update();
			Window::update();
		}
	}

	ObjectsManager::Instance().Clear();
	ResourceManager::deleteResources();
	Window::quit();
	UiHandler::onWindowClose();

	return 0;
}