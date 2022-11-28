#include "UiHandler.h"

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>


#include "../System/Window.h"

void UiHandler::onWindowCreate()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
   
	ImGui_ImplOpenGL3_Init();
	std::cout << "ImGui_ImplGlfw_InitForOpenGL: " << ImGui_ImplGlfw_InitForOpenGL(Window::window, true) << std::endl;
}

void UiHandler::onWindowClose()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void UiHandler::onDrawBegin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void UiHandler::onDrawEnd()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
