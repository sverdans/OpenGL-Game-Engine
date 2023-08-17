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

    ImGui_ImplOpenGL3_Init();
    bool lInit = ImGui_ImplGlfw_InitForOpenGL(Window::Instance().GetWindowPtr(), true);

    std::cout << "ImGui_ImplGlfw_InitForOpenGL: " << lInit << std::endl;

    const auto& window = Window::Instance();

    managersSidebarSize   = ImVec2(200, window.GetHeight());
    gameObjectSidebarSize = ImVec2(200, window.GetHeight());

    sidebarMinSize = ImVec2(200, window.GetHeight());
    sidebarMaxSize = ImVec2(400, window.GetHeight());

    NFD_Init();

    return lInit;
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

    /*if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
    {
        if (ImGuiFileDialog::Instance()->IsOk())
        {
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();

            ObjectsManager::deleteGameObjects();
            ResourceManager::deleteResources();

            ResourceManager::loadResources(filePathName);
            ObjectsManager::loadGameObjects(filePathName);

            BehaviourSystem::init();
            LightingSystem::update();
        }
        ImGuiFileDialog::Instance()->Close();
    }*/
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
                ImGui::Text(shader.first.c_str());
            }
        }

        if (ImGui::CollapsingHeader("Textures"))
        {
            for (const auto& texture : ResourceManager::Instance().mTextures)
            {
                ImGui::Text(texture.first.c_str());
            }
        }

        if (ImGui::CollapsingHeader("Material"))
        {
            for (const auto& material : ResourceManager::Instance().mMaterials)
            {
                ImGui::Text(material.first.c_str());
            }
        }

        if (ImGui::CollapsingHeader("Models"))
        {
            for (const auto& model : ResourceManager::Instance().mModels)
            {
                ImGui::Text(model.first.c_str());
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


