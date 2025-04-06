/**********************************************************************************
* \file

* \brief

* \author		Jeroen Tan Yi Xu

* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*				or disclosure of this file or its contents without the prior
*				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "ImGuiManager.h"
#include "ImGuizmo\ImGuizmo.h"
#include "Src\Engine\Systems\Graphics\Platform\OpenGLTexture.h"
#include "../Src/Engine/Core/Manager/PrefabManager.h"




namespace Eos
{
	//Helper Function to open window folder
	

	GfxImGui::MapNameToNode& GfxImGui::GetWindowsMap()
	{
		return m_layers;
	}
	void GfxImGui::DockSpaceWindow()
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);

		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		m_fullScreen = true;

		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		m_layerName = "EosEngine";
		m_dockspaceFlags |= ImGuiConfigFlags_DockingEnable;
		m_dockspaceFlags ^= ImGuiDockNodeFlags_AutoHideTabBar;


		static bool open = true;



		ImGui::Begin(m_layerName.c_str(), &open, window_flags);
		{
			MainMenuBar();
			PlayPauseStopButton();
		}

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;


		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), m_dockspaceFlags);
		ImGui::End();

		FileBrowser();

	}
	void GfxImGui::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
		//
		//	Disabing showDemo in releasedMode
		//
#ifndef _DEBUG
#else

		//
		// Commented out for submission -Jeroen
		// 
		//ImGui::ShowDemoWindow(); //Tools: might need to referenced from here
#endif
		ImGuiIO& io = ImGui::GetIO();
		saved_shift = io.KeyShift;
		saved_alt = io.KeyAlt;
		saved_ctrl = io.KeyCtrl;
		saved_super = io.KeySuper;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		DockSpaceWindow();
		

	}
	void GfxImGui::Init()
	{
		const char* glsl_version = "#version 460";

		IMGUI_CHECKVERSION();
		context = ImGui::CreateContext();


		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(Context::GetInstance().GetWindow(), true);
		ImGui_ImplOpenGL3_Init(glsl_version);

		isPlaying = false;
		m_PlayButton = Texture2D::Create("Buttons/PlayButton.png");
		m_PauseButton = Texture2D::Create("Buttons/PauseButton.png");
		m_StopButton = Texture2D::Create("Buttons/StopButton.png");

		m_fullScreen = true;
		m_layerName = "Default DockSpace";
		m_dockspaceFlags = ImGuiConfigFlags_DockingEnable;
		m_dockspaceFlags ^= ImGuiDockNodeFlags_AutoHideTabBar;

		ImGuiIO& io = ImGui::GetIO();
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiDockNodeFlags_AutoHideTabBar;
		io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

		//Setting the current path to be towards the data path
		//m_csharpdll = "EosScriptCore\\EosScriptCore\\bin\\Release\\netcoreapp3.1\\EosScriptCore.dll";
		//m_csharpdll = "EosScriptCore\\EosScriptCore\\bin\\Debug\\netcoreapp3.1\\EosScriptCore.dll";

		RegisterAllWindows();

		enum class window_type
		{
			GAMESCREEN_WINDOW = 0,
		};


		for (auto& layer : m_layers)
		{
			layer.second->SetActive(true);
		}	

	}
	void GfxImGui::Update()
	{
		for (auto& layer : m_layers)
		{
			if (layer.second->GetActive())
			{
				layer.second->Update();
			}
		}

		bool show_demo_window = false;
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
	}
	void GfxImGui::Exit()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	void GfxImGui::Render()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		ImGui::GetIO().KeyCtrl = saved_ctrl;
		ImGui::GetIO().KeyAlt = saved_alt;
		ImGui::GetIO().KeyShift = saved_shift;
		ImGui::GetIO().KeySuper = saved_super;

		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(Context::GetInstance().GetWindow(), &display_w, &display_h);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	bool GfxImGui::HotKeySaving() const
	{
		return Eos::Inputs::GetInstance().IsKeyPressed(Eos::KeyCode::LeftControl) &&
			Eos::Inputs::GetInstance().IsKeyPressed(Eos::KeyCode::LeftShift) &&
			Eos::Inputs::GetInstance().IsKeyPressed(Eos::KeyCode::S) &&
			(CoreManager::GetInstance().getEngineState() == ENGINESTATE::STOP);
	}
	bool GfxImGui::HotKeySaving(const std::string& currentscene) const
	{
		if (Eos::Inputs::GetInstance().IsKeyPressed(Eos::KeyCode::LeftControl) &&
			!Eos::Inputs::GetInstance().IsKeyPressed(Eos::KeyCode::LeftShift) &&
			Eos::Inputs::GetInstance().IsKeyPressed(Eos::KeyCode::S) &&
			(CoreManager::GetInstance().getEngineState() == ENGINESTATE::STOP))
		{
			return TrySaveScene(currentscene);
		}
		else
			return false;
	}
	bool GfxImGui::HotKeyLoading() const
	{
		return Eos::Inputs::GetInstance().IsKeyPressed(Eos::KeyCode::LeftControl) &&
			   Eos::Inputs::GetInstance().IsKeyPressed(Eos::KeyCode::O) &&
			   (CoreManager::GetInstance().getEngineState() == ENGINESTATE::STOP);
	}

	bool GfxImGui::TrySaveScene(const std::string& filename) const
	{
		UNREFERENCED_PARAMETER(filename);
		if (m_currentScene.empty())
			return true;


		PE_CORE_INFO("Scene Is Being Saved...");
		SceneManager::GetInstance().SaveScene(m_currentScene);
		

		return false;
	}

	void GfxImGui::RegisterAllWindows()
	{

		//TODO: Can be better , backend support for ImGuiWindow for faster prototyping
		RegisterWindow<GameScreenWindow>();
		RegisterWindow<UISceneWindow>();
		RegisterWindow<SceneHierachy>();
		RegisterWindow<LoggerWindow>();
		RegisterWindow<InspectorWindow>();
		RegisterWindow<DebugDataWindow>();
		//RegisterWindow<GraphicWindow>();
		//RegisterWindow<MaterialCompiler>();
		RegisterWindow<FileWindow>();
		//RegisterWindow<DepthWindow>();
		RegisterWindow<AudioManager>();
		RegisterWindow<AnimationWindow>();
	}
	void GfxImGui::DockSpaceSetUp()
	{
		m_fullScreen = true;
		m_layerName = "Default DockSpace";
		m_dockspaceFlags = ImGuiConfigFlags_DockingEnable;
		m_dockspaceFlags ^= ImGuiDockNodeFlags_AutoHideTabBar;

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	}

	void GfxImGui::FileBrowser()
	{
		{
			if (popupSceneLoad)
			{
				
				std::optional< std::string> m_filepath;
				m_filepath = DialogOpenFile(L"Open Scene", L"../Assets");
				if (m_filepath.has_value())
				{
					SceneManager::GetInstance().ReadScene(m_filepath.value());
				}
				else
				{
					PE_CORE_WARN("Cannot Read File");
				}
			}
		}
		
		{
			if (SceneManager::GetInstance().GetActiveScene().empty())
			{
				if (popupSceneSaver)
				{
					std::optional< std::string> m_filepath;
					m_filepath = DialogOpenFile(L"Save Scene As", L"../Assets");
					if (m_filepath.has_value())
					{
						SceneManager::GetInstance().SaveScene(m_filepath.value());
					}
					else
					{
						PE_CORE_WARN("Cannot Save File");
					}
				}
			}
		}
		{
			if (popupPrefabSaver)
			{
				std::optional< std::string> m_filepath;
				m_filepath = DialogOpenFile(L"Save Prefab As", L"../Assets/Data/Prefabs");
				if (m_filepath.has_value())
				{
					PrefabManager::GetInstance().SavePrefab(m_filepath.value());
				}
				else
				{
					PE_CORE_WARN("Cannot Save Prefab File");
				}
			}
			popupPrefabSaver = false;
		}
	}

	void GfxImGui::MainMenuBar()
	{
		m_currentScene = SceneManager::GetInstance().GetActiveScene();
		popupSceneSaver = HotKeySaving() || HotKeySaving(m_currentScene);
		popupSceneLoad = HotKeyLoading();

		ImGui::BeginMainMenuBar(); //TODO: all the main menu bar items
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New Scene", "Ctrl+n", false))
			{
				SceneManager::GetInstance().ClearScene();
				
			}

			if (ImGui::MenuItem("Open Scene", "Ctrl+o", false))
			{
				popupSceneLoad = true;
			}

			ImGui::Separator();

			if (ImGui::MenuItem("Save", "Ctrl+S", false))
			{
				popupSceneSaver = SceneManager::GetInstance().SaveScene(m_currentScene);
			}

			if (ImGui::MenuItem("Save As", "Shift+Ctrl+s", false))
			{
				popupSceneSaver = true;
			}


			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "Ctrl+z", false))
			{

			}

			if (ImGui::MenuItem("Redo", "Ctrl+y", false))
			{

			}

			ImGui::Separator();

			if (ImGui::MenuItem("Cut", "Ctrl+X", false))
			{

			}

			if (ImGui::MenuItem("Copy", "Ctrl+C", false))
			{

			}

			if (ImGui::MenuItem("Paste", "Ctrl+V", false))
			{

			}


			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::MenuItem("Create Empty"))
			{
				EntityManager::GetInstance().CreateEntity("");
			}

			if (ImGui::MenuItem("Edit Prefab"))
			{

			}

			if (ImGui::BeginMenu("Instanitate Prefab"))
			{
				ImGui::Separator();
				auto map = PrefabManager::GetInstance().GetPrefabMap();

				for (auto prefab : map)
				{
					if (ImGui::MenuItem(prefab.first.c_str()))
					{
						PrefabManager::GetInstance().registerPrefabToPrefabEntityMap(prefab.first);
					}
				}
				//if (ImGui::MenuItem("Red Tile"))
				//{
				//	PrefabManager::GetInstance().registerPrefabToPrefabEntityMap("red-tile");
				//}
				//if (ImGui::MenuItem("Blue Tile"))
				//{
				//	PrefabManager::GetInstance().registerPrefabToPrefabEntityMap("blue-tile");
				//}
				//if (ImGui::MenuItem("Yellow Tile"))
				//{
				//	PrefabManager::GetInstance().registerPrefabToPrefabEntityMap("yellow-tile");
				//}
				//// Current prefabs used now
				//if (ImGui::MenuItem("Gate"))
				//{
				//	PrefabManager::GetInstance().registerPrefabToPrefabEntityMap("gate");
				//}
				//if (ImGui::MenuItem("Yellow Data"))
				//{
				//	PrefabManager::GetInstance().registerPrefabToPrefabEntityMap("data-yellow");
				//}
				//if (ImGui::MenuItem("Blue Data"))
				//{
				//	PrefabManager::GetInstance().registerPrefabToPrefabEntityMap("data-blue");
				//}
				//if (ImGui::MenuItem("Red Data"))
				//{
				//	PrefabManager::GetInstance().registerPrefabToPrefabEntityMap("data-red");
				//}
				//if (ImGui::MenuItem("End Goal"))
				//{
				//	PrefabManager::GetInstance().registerPrefabToPrefabEntityMap("goal");
				//}
				ImGui::EndMenu();
			}
			// Another column to save prefab
			if (ImGui::MenuItem("Save Prefab"))
			{
				popupPrefabSaver = true;
				
			}


			if (ImGui::MenuItem("DrawDebug"))
			{
				std::shared_ptr<ISystem> renderingSystem = SystemManager::GetInstance().getSystem(SYSNAME::SYS_GRAPHICS);
				std::shared_ptr<Rendering_System>  rs = std::dynamic_pointer_cast<Rendering_System>(renderingSystem);
				rs->DrawDebug = !rs->DrawDebug;
			}




			ImGui::Separator();

			if (ImGui::MenuItem("2D Object"))
			{

			}

			if (ImGui::MenuItem("3D Object"))
			{

			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Component"))
		{
			//List 
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Build"))
		{
			if (ImGui::MenuItem("Toggle Debug"))
			{
				std::shared_ptr<ISystem> is = SystemManager::GetInstance().getSystem(SYSNAME::SYS_GRAPHICS);
				std::shared_ptr<Rendering_System> rs = std::dynamic_pointer_cast<Rendering_System>(is);
				rs->DrawDebug = !(rs->DrawDebug);
			}

			if (ImGui::MenuItem("Toggle Picking"))
			{
				static bool pickingFlag = true;
				pickingFlag = !pickingFlag;
				//	Set event to MousePicker
				//
				SetPickingEvent SetPickingEvent{ pickingFlag };
				PE_PUBLISH_EVENT(SetPickingEvent);
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{

			if (ImGui::MenuItem("About"))
				showPopUp = true;
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Display windows"))
		{
			// List out all layers as clickable elements
			for (auto& layer : m_layers)
			{
				if (ImGui::MenuItem(layer.first.data()))
				{
					layer.second->SetActive(true);
				}
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();

		
	

	}

	void GfxImGui::PlayPauseStopButton()
	{
		// if not playing
		//		show play button
		// if playing
		//		show pause and stop button
		if (CoreManager::GetInstance().getEngineState() == ENGINESTATE::PLAY)
			isPlaying = true;
		else
			isPlaying = false;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 imageSize{ 20,20 };

		std::shared_ptr<Rendering_System>  rs = std::dynamic_pointer_cast<Rendering_System>(SystemManager::GetInstance().getSystem(SYSNAME::SYS_GRAPHICS));

		if (isPlaying)
		{
			ImGui::SameLine(viewport->WorkSize.x / 2.05f);
			if (ImGui::ImageButton((ImTextureID)m_PauseButton->GetID(), imageSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f)))
			{
				isPlaying = false;
				CoreManager::GetInstance().setEngineState(ENGINESTATE::PAUSE);
				rs->m_persCamera.m_active = true;
			}
			ImGui::SameLine();
			if (ImGui::ImageButton((ImTextureID)m_StopButton->GetID(), imageSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f)))
			{
				isPlaying = false;
				//Insert code here to deserialize scene
				rs->m_persCamera.m_active = true;
				SceneManager::GetInstance().ReadPlayScene("temp.json");
				CoreManager::GetInstance().setEngineState(ENGINESTATE::STOP);
			}
		}
		else // not playing
		{
			ImGui::SameLine(viewport->WorkSize.x / 2.05f);
			if (ImGui::ImageButton((void*)m_PlayButton->GetID(), imageSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f)))
			{
				isPlaying = true;
				//Eos::ScriptSystem::ReloadAssembly(m_csharpdll);
				Eos::ScriptSystem::OnRuntimeStart();
				//Only save the scene if the prev state was STOP
				if (CoreManager::GetInstance().getEngineState() == ENGINESTATE::STOP) 
				{
					//Insert code here to serialize scene
					SceneManager::GetInstance().SaveScene("temp.json");
				}
				rs->m_persCamera.m_active = false;
				CoreManager::GetInstance().setEngineState(ENGINESTATE::PLAY);
				
				
				
			}
			ImGui::SameLine();
			if (ImGui::ImageButton((void*)m_StopButton->GetID(), imageSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f)))
			{
				isPlaying = false;
			}

		}

		//ImGui::ImageButton()

	}

}