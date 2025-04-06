/**********************************************************************************
* \file

* \brief

* \author		Jeroen Tan Yi Xu

* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*				or disclosure of this file or its contents without the prior
*				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once
#include "ImGuiFileBrowser.h"
#include "..\pch.h"
#include "IImGuiWindow.h"
#include "Context.h"
#include "Tools\Singleton.h"
#include <unordered_map>
#include <memory>
#include "GameScreenWindow.h"
#include "UISceneWindow.h"
#include "SceneHierachy.h"
#include "InspectorWindow.h"
#include "LoggerWindow.h"
#include "DebugDataWindow.h"
#include "GraphicWindow.h"
#include "MousePicking.h"
#include "MaterialCompiler.h"
#include "FileWindow.h"
#include "DepthWindow.h"
#include "AudioManager.h"
#include "AnimationWindow.h"
#include "../Src/Engine/Core/Manager/SceneManager.h"
#include "../Src/Engine/Systems/Graphics/Renderer/Texture.h"



namespace Eos
{
    class GfxImGui final : public Singleton<GfxImGui>
    {
    public:

        //map the name to the correct imguiWindow
        using MapNameToNode = std::unordered_map<std::string_view, std::shared_ptr<IImGuiWindow>>;
        MapNameToNode& GetWindowsMap();
        void														 MainMenuBar();
        void														 PlayPauseStopButton();
        void														 DockSpaceWindow();
        void														 Begin();
        void														 Init();
        void														 Update();
        void														 Exit();
        void														 Render();


        bool                                                        HotKeySaving() const;
        bool                                                        HotKeySaving(const std::string& filename) const;
        bool                                                        HotKeyLoading() const;

        bool														isPlaying = false;
        bool                                                        TrySaveScene(const std::string& filename) const;


    private:
        MapNameToNode												 m_layers;
        bool														 m_fullScreen = false;
        std::string                                                  m_currentScene;
        std::string													 m_layerName;
        ImGuiDockNodeFlags m_dockspaceFlags;
        ImGuiContext* context{ nullptr };

        void														RegisterAllWindows();
        void														DockSpaceSetUp();
        void														FileBrowser();
        bool														popupSceneSaver = false;
        bool														popupSceneLoad = false;
        bool                                                        popupPrefabSaver = false;
        bool                                                        saved_ctrl = false;
        bool                                                        saved_alt = false;
        bool                                                        saved_shift = false;
        bool                                                        saved_super = false;

        // Buttons
        std::shared_ptr<Texture2D>								    m_PlayButton;
        std::shared_ptr<Texture2D>								    m_PauseButton;
        std::shared_ptr<Texture2D>								    m_StopButton;
        std::string                                                 m_csharpdll;

        template<typename TypeOfWindow>
        void RegisterWindow();

        bool showPopUp = false;

    };
}

#include "ImGuiManager.hpp"