#include "CoreManager.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/attrdefs.h>
#include <filesystem>
#include "Src\Engine\Systems\Graphics\EosRenderer\FileWatcher.h"
#include "Src\Engine\Core\Manager\AssetManager.h"

CoreManager::CoreManager() {}

void CoreManager::Init()
{
    {
        //
        //	Set up all the required tools
        //
        Eos::Context::GetInstance().Init(1280, 800);
        Eos::EosEditorAssets::GetInstance().Init();
        Eos::FrameRateManager::GetInstance().Init();
        Eos::GfxImGui::GetInstance().Init();

        //Add to Internal Call, remove from here
        Eos::Inputs::GetInstance().JoystickInit();

        HistoryManager::GetInstance().Init();
        SystemManager::GetInstance().InitAllSystems();
        SceneManager::GetInstance().Init();
        PrefabManager::GetInstance().Init();
        AssetManager::GetInstance().Init();
    }
}

void CoreManager::Update()
{
    while (isRunning && !glfwWindowShouldClose(Eos::Context::GetInstance().GetWindow()))
    {

        //
        //  Get deltaTime (not fixed deltaTime)
        //
        {

            //GLfloat deltaTime = Eos::FrameRateManager::GetInstance().delta();
            //std::cout << deltaTime << std::endl;
        }
#ifndef _DEBUG
        //ZoneScopedN("Main Loop")
#else

#endif



        //Ask before you change the order of this -Jeroen
        Eos::GfxImGui::GetInstance().Begin();
        static int counter = 0;

        Eos::FrameRateManager::GetInstance().Start();
        Eos::FrameRateManager::GetInstance().Update(); // get correct deltaTime here
        
        Eos::Context::GetInstance().Update();
        HistoryManager::GetInstance().Update();
        PrefabManager::GetInstance().Update();
		SystemManager::GetInstance().UpdateAllSystems();
		GET_COMPONENT_MANAGER.calculateTransformOffset();
		GET_COMPONENT_MANAGER.parentChildUpdate();
		GET_COMPONENT_MANAGER.updatePrevTransform();

		glViewport(
			0,
			0,
			Eos::Context::GetInstance().scr_width,
			Eos::Context::GetInstance().scr_height);

        Eos::GfxImGui::GetInstance().Update();
        Eos::GfxImGui::GetInstance().Render();
        Eos::Context::GetInstance().PostUpdate(); //to swap buffers


        SceneManager::GetInstance().Update();
        //Eos::FrameRateManager::GetInstance().Start();


        if (Eos::Inputs::GetInstance().IsKeyPressed(Eos::KeyCode::Escape))
        {
            isRunning = false; //Key press and mouse button is working correctly
        }

		
		Eos::FrameRateManager::GetInstance().Start();
		////Input is working correctly, TODO button and key pressed events
		//std::cout << "[Mouse Pos]: " << Eos::Inputs::GetInstance().GetMousePosition().first << " "
		//	<< Eos::Inputs::GetInstance().GetMousePosition().second << std::endl;
		//
		////FrameRate Manager is working correctly
		//std::cout << "[FPS]: " <<
		//	Eos::FrameRateManager::GetInstance().GetFrameRate() << std::endl;
		//
		//if (Eos::Inputs::GetInstance().IsMouseButtonPressed(Eos::MouseButton::Right))
		//{
		//	isRunning = false;
		//}

        //
        // A Simple Filewatcher
        //
        // 

        {
            static editor::FileWatcher fw("Assets\\Data");
            //fw.OnUpdate();
        }

        {
            std::stringstream m_windowTitle;
            m_windowTitle << "EosEngine";
            glfwSetWindowTitle(Eos::Context::GetInstance().GetWindow(), m_windowTitle.str().c_str());
        }

#ifndef _DEBUG
        //FrameMark
#endif

    }
}

void CoreManager::Exit()
{
    SystemManager::GetInstance().ExitAllSystems();
    Eos::GfxImGui::GetInstance().Exit();
}

ENGINESTATE CoreManager::getEngineState()
{
    return currState;
}

void CoreManager::setEngineState(ENGINESTATE engineState)
{
    currState = engineState;
}
