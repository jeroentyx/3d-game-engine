
#include "SceneManager.h"
#include "Src\Engine\Core\Manager\EntitiesIDManager.h"
#include "..\Src\pch.h"
#include "Src\Engine\Systems\Graphics\Compiler\TextureLoader.h"
#include <filesystem>

SceneManager::SceneManager()
{
    PE_CORE_INFO("Scene Manager Created!");
}

SceneManager::~SceneManager()
{

}

void SceneManager::Init()
{
    
    
}

void SceneManager::Update()
{
    //Will change it in future
    Eos::Inputs::GetInstance().TestController();

    //Detect the change in Scene and Unload Accordingly
    if (FileManager::Exists(m_nextScene))
    {
        ScriptSystem::OnRuntimeStop();
        ReadScene(m_nextScene);
        ScriptSystem::OnRuntimeStart();
        m_nextScene.clear();
        CoreManager::GetInstance().setEngineState(ENGINESTATE::PLAY);
    }
}

void SceneManager::Exit()
{
}

void SceneManager::ReadPlayScene(const std::string& filename)
{
    EntityManager::GetInstance().ClearAllEntities();
    ComponentManager::GetInstance().ClearAllComponents();

    JsonReader _reader;
    //Assert if the filename exists or not
    EosAssert(Eos::FileManager::Exists(filename), "Filename Does Not Exist");
    //Save the current scene

    std::string buffer;
    buffer = Eos::FileManager::ReadFile(filename);
    EntityID test;
    EntityManager::GetInstance().Load(buffer, _reader, test);
}

void SceneManager::ReadScene(const std::string& filename)
{
    ClearScene();
    JsonReader _reader;
    //Assert if the filename exists or not
    EosAssert(Eos::FileManager::Exists(filename), "Filename Does Not Exist");
    //Save the current scene

    m_currentScene = filename;

    std::string buffer;
    buffer = Eos::FileManager::ReadFile(filename);
    EntityID test;
    EntityManager::GetInstance().Load(buffer, _reader, test);
}

bool SceneManager::SaveScene(const std::string& filename)
{
    EntityID test;
    JsonWriter _writer;
    if (EntityManager::GetInstance().Save("", _writer, test))
    {
        Eos::FileManager::WriteFile(filename, _writer.GetString());
        return true;
    }
    

    return false;
}

std::string& SceneManager::GetActiveScene()
{
    return m_currentScene;
}

void SceneManager::SetNextScene(std::string& next)
{
    m_nextScene = next;
}

void SceneManager::ClearScene()
{
    EntityManager::GetInstance().ClearAllEntities();
    ComponentManager::GetInstance().ClearAllComponents();
    m_currentScene.clear();
}
