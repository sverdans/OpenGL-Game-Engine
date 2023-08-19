#include <nlohmann/json.hpp>
#include <nfd.hpp>

#include <Editor/Editor.h>
#include <System/Window.h>

#include <Game/GameObject.h>
#include <Game/ObjectsManager.h>
#include <Game/ResourceManager.h>

#include <Game/Systems/BehaviourSystem.h>
#include <Game/Systems/LightingSystem.h>

bool Editor::Initialize()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    auto& window = Window::Instance();

    if (!ImGui_ImplOpenGL3_Init())
    {
        spdlog::get("main")->critical("ImGui_ImplOpenGL3_Init failed");
        return false;
    }

    if (!ImGui_ImplGlfw_InitForOpenGL(window.mpWindow, true))
    {
        spdlog::get("main")->critical("ImGui_ImplGlfw_InitForOpenGL failed");
        ImGui_ImplOpenGL3_Shutdown();
        return false;
    }

    if (NFD_Init() == NFD_ERROR)
    {
        spdlog::get("main")->critical(std::string("NFD_Init failed: ") + NFD_GetError());
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        return false;
    }

    managersSidebarSize   = ImVec2(200, window.GetHeight());
    gameObjectSidebarSize = ImVec2(200, window.GetHeight());

    sidebarMinSize = ImVec2(200, window.GetHeight());
    sidebarMaxSize = ImVec2(400, window.GetHeight());

    SetupStyle();
    return true;
}

void Editor::Finalize()
{
    NFD_Quit();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Editor::OnDrawBegin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Editor::OnDrawEnd()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::SetupStyle()
{
    auto& style = ImGui::GetStyle();

    style.WindowPadding     = {  5, 5 };    
    style.FramePadding      = {  5, 3 };    
    style.CellPadding       = {  5, 3 };    
    style.ItemSpacing       = { 10, 3 };
    style.ItemInnerSpacing  = {  5, 5 };
    style.TouchExtraPadding = {  0, 0 };

    style.IndentSpacing = 20;
    style.ScrollbarSize = 15;
    style.GrabMinSize   = 15;
    
    style.WindowBorderSize  =  0;
    style.ChildBorderSize   =  0;
    style.PopupBorderSize   =  0;
    style.FrameBorderSize   =  0;
    style.TabBorderSize     =  0;

    style.WindowRounding    =  0;
    style.ChildRounding     =  5;
    style.FrameRounding     =  5;
    style.ScrollbarRounding = 10;
    style.GrabRounding      =  5;
    style.TabRounding       =  5;

    style.WindowTitleAlign = { 0.5f, 0.5f };
}

void Editor::Update()
{
    const auto& window = Window::Instance();

    sidebarMinSize = ImVec2(200, window.GetHeight());
    sidebarMaxSize = ImVec2(400, window.GetHeight());

    OnDrawBegin();

    if (!isGameObjectSelected)
        currentGameObject = nullptr;

//	ImGui::ShowDemoWindow();

    DrawMenuStrip();
    DrawManagersSidebar();
    DrawGameObjectSidebar();

    OnDrawEnd();
}



void Editor::PresentJsonObject(json& jsonObject)
{
    ImGui::Indent(indentValue);

    for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it)
    {
        std::string tempInputUID = std::string("##" + inputUID + std::string(it.key()));

        std::string type = it.value().type_name();

        if (type == "number")
        {
            float f = it.value();

        //	if (ImGui::InputFloat(tempInputUID.c_str(), &f) && it.value() != f)
            if (ImGui::DragFloat(tempInputUID.c_str(), &f, 0.005f, -FLT_MAX, +FLT_MAX) && it.value() != f)
            {
        //		glfwcursorhid
                needDeserializeCurrentObject = true;
                it.value() = f;
            }
            ImGui::SameLine();
            ImGui::Text("%s", it.key().c_str());
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
            ImGui::Text("%s", it.key().c_str());
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
            ImGui::Text("%s", it.key().c_str());
        }

        if (type == "object")
        {
            if (ImGui::CollapsingHeader(it.key().c_str(), ImGuiTreeNodeFlags_FramePadding))
            {
                inputUID += it.key();
                PresentJsonObject(it.value());
            }
        }
    }

    ImGui::Unindent(indentValue);
}

void Editor::DrawMenuStrip()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open"))
            {
            //	ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose Project file", ".json", ".");
                
                NFD::UniquePath outPath;
                nfdfilteritem_t filterItem[1] = { {"Projects file", "json"} };

                nfdresult_t result = NFD::OpenDialog(outPath, filterItem, 1);

                if (result == NFD_OKAY)
                {
                    std::cout << "Success!" << std::endl << outPath.get() << std::endl;

                    ObjectsManager::Instance().Clear();
                    ResourceManager::Instance().Clear();

                    ResourceManager::Instance().Load(outPath.get());
                    ObjectsManager::Instance().Load(outPath.get());

                    BehaviourSystem::Instance().Init();
                    LightingSystem::Instance().Update();
                }
                else if (result == NFD_CANCEL)
                {
                    std::cout << "User pressed cancel." << std::endl;
                }
                else
                {
                    std::cout << "Error: " << NFD::GetError() << std::endl;
                }
            }
            
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
}

void Editor::PresentResourceManager()
{
    if (ImGui::CollapsingHeader("Resources"))
    {
        ImGui::Indent(indentValue);
        if (ImGui::CollapsingHeader("Shaders"))
        {
            for (const auto& shader : ResourceManager::Instance().mShaders)
            {
                ImGui::Text("%s", shader.first.c_str());
            }
        }

        if (ImGui::CollapsingHeader("Textures"))
        {
            for (const auto& texture : ResourceManager::Instance().mTextures)
            {
                ImGui::Text("%s", texture.first.c_str());
            }
        }

        if (ImGui::CollapsingHeader("Material"))
        {
            for (const auto& material : ResourceManager::Instance().mMaterials)
            {
                ImGui::Text("%s", material.first.c_str());
            }
        }

        if (ImGui::CollapsingHeader("Models"))
        {
            for (const auto& model : ResourceManager::Instance().mModels)
            {
                ImGui::Text("%s", model.first.c_str());
            }
        }
    
        ImGui::Unindent(indentValue);
    }
}

void Editor::PresentObjectsManager()
{
    if (ImGui::CollapsingHeader("GameObjects"))
    {
        for (const auto& object : ObjectsManager::Instance().mGameObjects)
        {
            if (ImGui::Selectable(object->GetName().c_str(), object == currentGameObject))
            {
                currentGameObject = object;
                isGameObjectSelected = true;
            }
        }
    }
}

void Editor::DrawManagersSidebar()
{
    ImGui::SetNextWindowPos(ImVec2(0, 18));
    ImGui::SetNextWindowSizeConstraints(sidebarMinSize, sidebarMaxSize);
    ImGui::SetNextWindowSize(managersSidebarSize);

    if (ImGui::Begin("Managers", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
    {
        managersSidebarSize = ImVec2(ImGui::GetWindowSize().x, Window::Instance().GetHeight());

        PresentObjectsManager();
        PresentResourceManager();

        ImGui::End();
    }
}

void Editor::DrawGameObjectSidebar()
{
    if (!isGameObjectSelected)
        return;

    const auto& window = Window::Instance();

    ImGui::SetNextWindowPos(
        ImVec2(
            window.GetWidth() - gameObjectSidebarSize.x,
            18));
    ImGui::SetNextWindowSizeConstraints(sidebarMinSize, sidebarMaxSize);
    ImGui::SetNextWindowSize(gameObjectSidebarSize);

    if (ImGui::Begin("GameObject", &isGameObjectSelected, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
    {
        gameObjectSidebarSize = ImVec2(ImGui::GetWindowSize().x, window.GetHeight());
    
        json jsonGameObject = currentGameObject->Serialize();

        needDeserializeCurrentObject = false;
        inputUID.clear();

        ImGui::Unindent(indentValue);
        PresentJsonObject(jsonGameObject);

        if (needDeserializeCurrentObject)
        {
            currentGameObject->Deserialize(jsonGameObject);
        }

        ImGui::End();
    }
}


