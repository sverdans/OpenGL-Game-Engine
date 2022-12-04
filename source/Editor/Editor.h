#include <limits>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <nlohmann/json.hpp>

#include "UiHandler.h"
#include "../Resources/ResourceManager.h"
#include "../Game/ObjectsManager.h"
#include "../Game/GameObject.h"
#include "../Editor/ImGuiFileDialog/ImGuiFileDialog.h"

#include "../Game/Systems/LightingSystem.h"


class Editor
{
private:

	static constexpr int indentValue = 10;
	static ImVec2 managersSidebarSize;
	static ImVec2 gameObjectSidebarSize;
	static ImVec2 sidebarMinSize;
	static ImVec2 sidebarMaxSize;
	
	static GameObject* currentGameObject;
	static bool isGameObjectSelected;
	static bool needDeserializeCurrentObject;
	static std::string inputUID;

	static void writeInFile()
	{

	}

	static void jsonObject(nlohmann::json& jsonValue)
	{
		ImGui::Indent(indentValue);

		for (auto it = jsonValue.begin(); it != jsonValue.end(); ++it)
		{
			std::string tempInputUID = std::string("##" + inputUID + std::string(it.key()));

			std::string type = it.value().type_name();

			if (type == "number")
			{
				float f = it.value();

			//	if (ImGui::InputFloat(tempInputUID.c_str(), &f) && it.value() != f)
				if (ImGui::DragFloat(tempInputUID.c_str(), &f, 0.005f, -FLT_MAX, +FLT_MAX) && it.value() != f)
				{
					needDeserializeCurrentObject = true;
					it.value() = f;
				}
				ImGui::SameLine();
				ImGui::Text(it.key().c_str());
			}

			if (type == "string")
			{
				std::string s = it.value();

				if (ImGui::InputText(tempInputUID.c_str(), &s[0], 20) && it.value() != s)
				{
					needDeserializeCurrentObject = true;
					it.value() = s;
				}
				ImGui::SameLine();
				ImGui::Text(it.key().c_str());
			}

			if (type == "boolean")
			{
				bool b = it.value();
				if (ImGui::Checkbox(tempInputUID.c_str(), &b) && it.value() != b)
				{
					needDeserializeCurrentObject = true;
					it.value() = b;
				}
				ImGui::SameLine();
				ImGui::Text(it.key().c_str());
			}

			if (type == "object")
			{
				if (ImGui::CollapsingHeader(it.key().c_str(), ImGuiTreeNodeFlags_FramePadding))
				{
					inputUID += it.key();
					jsonObject(it.value());
				}
			}
		}

		ImGui::Unindent(indentValue);
	}

	static void menuStrip()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open"))
				{
					ImVec2 size = ImVec2(Window::size.x - 100, Window::size.y - 100);
					ImGui::SetNextWindowPos(ImVec2(50, 50));
					ImGui::SetNextWindowSize(size);

					ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose Project file", ".json", ".");
				}
				
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Renderer"))
			{
				Renderer::drawTools();
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
		{
			if (ImGuiFileDialog::Instance()->IsOk())
			{
				std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();

				ObjectsManager::deleteGameObjects();
				ResourceManager::deleteAllResources();

				ResourceManager::loadResources(filePathName);
				ObjectsManager::loadGameObjects(filePathName);

				BehaviourSystem::init();
				LightingSystem::update();
			}
			ImGuiFileDialog::Instance()->Close();
		}
	}

	static void resourceManager()
	{
		if (ImGui::CollapsingHeader("Resources"))
		{
			ImGui::Indent(indentValue);
			if (ImGui::CollapsingHeader("Shaders"))
			{
				for (const auto& shader : ResourceManager::shaders)
				{
					ImGui::Text(shader.first.c_str());
				}
			}

			if (ImGui::CollapsingHeader("Textures"))
			{
				for (const auto& texture : ResourceManager::textures)
				{
					ImGui::Text(texture.first.c_str());
				}
			}

			if (ImGui::CollapsingHeader("Models"))
			{
				for (const auto& model : ResourceManager::models)
				{
					ImGui::Text(model.first.c_str());
				}
			}
		
			ImGui::Unindent(indentValue);
		}
	}

	static void gameObjectsManager()
	{
		if (ImGui::CollapsingHeader("GameObjects"))
		{
			for (const auto& object : ObjectsManager::gameObjects)
			{
				if (ImGui::Selectable(object->name.c_str(), object == currentGameObject))
				{
					currentGameObject = object;
					isGameObjectSelected = true;
				}
			}
		}
	}

	static void managersSidebar()
	{
		ImGui::SetNextWindowPos(ImVec2(0, 18));
		ImGui::SetNextWindowSizeConstraints(sidebarMinSize, sidebarMaxSize);
		ImGui::SetNextWindowSize(managersSidebarSize);

		if (ImGui::Begin("Managers", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
		{
			managersSidebarSize = ImVec2(ImGui::GetWindowSize().x, Window::size.y);

			gameObjectsManager();
			resourceManager();

			ImGui::End();
		}
	}

	static void gameObjectSidebar()
	{
		if (!isGameObjectSelected)
			return;

		ImGui::SetNextWindowPos(ImVec2(Window::size.x - gameObjectSidebarSize.x, 18));
		ImGui::SetNextWindowSizeConstraints(sidebarMinSize, sidebarMaxSize);
		ImGui::SetNextWindowSize(gameObjectSidebarSize);

		if (ImGui::Begin("GameObject", &isGameObjectSelected, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
		{
			gameObjectSidebarSize = ImVec2(ImGui::GetWindowSize().x, Window::size.y);
		
			nlohmann::json jsonGameObject;
			currentGameObject->serialize(jsonGameObject);

			needDeserializeCurrentObject = false;
			inputUID.clear();

			ImGui::Unindent(indentValue);
			jsonObject(jsonGameObject);

			if (needDeserializeCurrentObject)
			{
				currentGameObject->deserialize(jsonGameObject);
			}

			ImGui::End();
		}
	}

public:
	static void init()
	{
		managersSidebarSize = ImVec2(200, Window::size.y);
		gameObjectSidebarSize = ImVec2(200, Window::size.y);

		sidebarMinSize = ImVec2(200, Window::size.y);
		sidebarMaxSize = ImVec2(400, Window::size.y);
	}

	static void update()
	{
		sidebarMinSize = ImVec2(200, Window::size.y);
		sidebarMaxSize = ImVec2(400, Window::size.y);

		UiHandler::onDrawBegin();

		if (!isGameObjectSelected) currentGameObject = nullptr;

		ImGui::ShowDemoWindow();

		menuStrip();
		managersSidebar();
		gameObjectSidebar();

		UiHandler::onDrawEnd();
	}
};


ImVec2 Editor::managersSidebarSize;
ImVec2 Editor::gameObjectSidebarSize;

ImVec2 Editor::sidebarMinSize;
ImVec2 Editor::sidebarMaxSize;

bool Editor::isGameObjectSelected = false;
bool Editor::needDeserializeCurrentObject = false;

GameObject* Editor::currentGameObject;
std::string Editor::inputUID;
