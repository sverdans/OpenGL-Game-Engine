#ifndef IMGUI_DEFINE_MATH_OPERATORS
	#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include <string>

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_glfw.h>

#include <nlohmann/json.hpp>

using json = nlohmann::ordered_json;

class GameObject;

class Editor
{
private:
	void PresentJsonObject(json& jsonValue);
	void PresentResourceManager();
	void PresentObjectsManager();

	void DrawManagersSidebar();
	void DrawGameObjectSidebar();
	void DrawMenuStrip();

public:
	static Editor& Instance()
	{
		static Editor instance;
		return instance;
	}

	bool Initialize();
	void Update();
	void Finalize();

private:
	Editor() = default;
	~Editor() = default;
	Editor(const Editor&) = delete;
	Editor(const Editor&&) = delete;
	Editor& operator = (const Editor&) = delete;
	Editor& operator = (const Editor&&) = delete;

private:
	void OnDrawBegin();
	void OnDrawEnd();

private:
	const int indentValue = 10;
	ImVec2 managersSidebarSize;
	ImVec2 gameObjectSidebarSize;
	ImVec2 sidebarMinSize;
	ImVec2 sidebarMaxSize;

	GameObject* currentGameObject;
	bool isGameObjectSelected;
	bool needDeserializeCurrentObject;
	std::string inputUID;
};
