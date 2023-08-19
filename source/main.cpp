#include <iostream>
#include <typeinfo>
#include <vector>
#include <map>

#include <spdlog/spdlog.h>
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

bool SetupLogger()
{
	try 
	{
		auto pLogger = spdlog::basic_logger_mt("main", "logs/log.log", true);
	}
	catch (const spdlog::spdlog_ex &ex)
	{
		std::cout << "Log init failed: " << ex.what() << std::endl;
		return false;
	}

	spdlog::get("main")->info("Logger initilized");
	return true;
}

int main(int argc, char** argv)
{
	if (!SetupLogger())
		return -1;
	
	auto& window = Window::Instance(); 
	if (!window.Initialize(1000, 800, "OpenGL-Courswork"))
	{
		window.Finalize();
		return -1;
	}

	auto& editor = Editor::Instance();
	if (!editor.Initialize())
	{
		editor.Finalize();
		window.Finalize();
		return -1;
	}

	Renderer::setClearColor(60.f / 255.f , 60.f / 255.f, 60.f / 255.f, 0.f);
	Renderer::enableDepthTest();

	InputHandler::init();

	LightingSystem::Instance().setAmbientColor({ 1.f, 1.f, 1.f });
	LightingSystem::Instance().setAmbientStrength(0.225f);
	
	Time::setUpdateFrequency(60.0);
	Time::start();

	while (window.ShouldRun())
	{
		Time::update();

		if (Time::CheckFPS())
		{
			Renderer::clear();

			BehaviourSystem::Instance().Update();
			BehaviourSystem::Instance().LateUpdate();
			BehaviourSystem::Instance().OnPreRender();

			LightingSystem::Instance().Update();
			RendererSystem::Instance().Render();

			editor.Update();
			window.Update();
		}
	}

	ObjectsManager::Instance().Clear();
	ResourceManager::Instance().Clear();

	editor.Finalize();
	window.Finalize();

	return 0;
}