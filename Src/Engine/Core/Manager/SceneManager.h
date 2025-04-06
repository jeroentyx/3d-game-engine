#pragma once

#include <string>
#include "Tools\Singleton.h"
#include "Src\ImGui\SceneHierachy.h"
#include "Src\Engine\Systems\Graphics\EosRenderer\EosEditorResources.h"
#include "Src/pch.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "../Src/Engine/Systems/Logic/ScriptingSystem.hpp"

class ComponentManager;
class EntityManager;

class SceneManager : 
	public Singleton<SceneManager>
{
	JsonReader m_reader;
	JsonWriter m_writer;

	std::string m_currentScene;
	std::string m_nextScene;

public:
	SceneManager();
	~SceneManager();



	void Init();
	void Update();
	void Exit();

	//Loading and Saving
	void ReadPlayScene(const std::string& filename);
	void ReadScene(const std::string& filename);
	bool SaveScene(const std::string& filename);
	

	//Used to clear all entities and components
	void ClearScene();

	//Getter And Setter
	std::string& GetActiveScene();
	void SetNextScene(std::string& next);




};
