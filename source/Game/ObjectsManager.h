#pragma once
#include <map>
#include <list>
#include <string>

#include <Game/Component.h>
#include <Game/GameObject.h>

#define DECLARE_COMPONENT(NAME) \
bool l ## NAME = ObjectsManager::Instance().DeclareComponent<NAME>();

class ObjectsManager final
{
    friend class Editor;
public:
    static ObjectsManager& Instance() 
    { 
        static ObjectsManager instance;
        return instance;
    }

    template<typename T>
    bool DeclareComponent()
    {
        std::string sName { std::string(typeid(T).name()).substr(2) };
        // info        
        auto pComponent = mpExample->AddComponent<T>();
        mComponents.emplace(sName, pComponent);
        return true;
    }

    void Clear();
    void Load(const std::string& sFilePath);

    GameObject* FindByTag(const std::string sName);
    GameObject* FindByName(const std::string sName);
    Component* GetComponentInstance(const std::string& sName);

private:
    ObjectsManager();
    ~ObjectsManager() = default;
    ObjectsManager(const ObjectsManager&) = delete;
    ObjectsManager(const ObjectsManager&&) = delete;
    ObjectsManager& operator = (const ObjectsManager&) = delete;
    ObjectsManager& operator = (const ObjectsManager&&) = delete;

private:
    GameObject* mpExample;
    std::list<GameObject*> mGameObjects;
    std::map<std::string, Component*> mComponents;
};
