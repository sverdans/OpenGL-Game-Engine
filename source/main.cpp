#include <iostream>
#include <typeinfo>
#include <vector>
#include <map>

#include <spdlog/sinks/basic_file_sink.h>

#include <System/Window.h>
#include <System/Time.h>
#include <System/InputHandler.h>

#include <Renderer/Renderer.h>

#include <Game/ResourceManager.h>
#include <Game/ObjectsManager.h>
#include <Game/Systems/RendererSystem.h>
#include <Game/Systems/BehaviourSystem.h>
#include <Game/Systems/LightingSystem.h>

#include <Editor/Editor.h>

void basic_logfile_example()
{
    try 
    {
        auto logger = spdlog::basic_logger_mt("basic_logger", "logs/basic-log.txt");
    }
    catch (const spdlog::spdlog_ex &ex)
    {
        std::cout << "Log init failed: " << ex.what() << std::endl;
    }
}

int main(int argc, char** argv)
{
	basic_logfile_example();

	Window::Instance().Initialize(1000, 800, "OpenGL-Courswork");

	Renderer::setClearColor(60.f / 255.f , 60.f / 255.f, 60.f / 255.f, 0.f);

//	Renderer::setClearColor(0.f, 0.f, 0.f, 0.f);
	Renderer::enableDepthTest();

	InputHandler::init();

	Editor::Instance().Initialize();

	LightingSystem::Instance().setAmbientColor(glm::vec3(1.f, 1.f, 1.f));
	LightingSystem::Instance().setAmbientStrength(0.225f);

	Time::setUpdateFrequency(60.0);
	Time::start();

	while (!Window::Instance().NeedFinalize())
	{
		glfwPollEvents();

		Time::update();

		if (Time::CheckFPS())
		{
			Renderer::clear();

			BehaviourSystem::Instance().Update();
			BehaviourSystem::Instance().LateUpdate();
			BehaviourSystem::Instance().OnPreRender();

			LightingSystem::Instance().Update();
			RendererSystem::Instance().Render();

			Editor::Instance().Update();
			Window::Instance().Update();
		}
	}

	ObjectsManager::Instance().Clear();
	ResourceManager::Instance().Clear();
	
	Window::Instance().Finalize();
	Editor::Instance().Finalize();

	return 0;
}