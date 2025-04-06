#pragma once

#include "Src\Engine\Systems\Graphics\EosRenderer\FileWatcher.h"
#include "..\Core\Manager\ComponentManager.h"
#include "..\Core\Manager\PrefabManager.h"
#include "..\Core\Manager\EntityManager.h"
#include "..\Core\Manager\SystemManager.h"
#include "..\Core\Manager\SceneManager.h"
#include "..\Src\Profiler\FrameRate.h"
#include "..\..\Src\ImGui\Context.h"
#include "..\..\Src\ImGui\GfxGui.h"
#include "Src\Inputs\Inputs.h"
#include "Src\ImGui\ImGuiManager.h"
#include "Src\Engine\Systems\Graphics\EosRenderer\EosEditorResources.h"
#include "Tools\Singleton.h"
#include "../Src/Engine/Systems/Logic/ScriptingSystem.hpp"
#include <Src/pch.h>
#include <tracy/Tracy.hpp>
#include "Src\Engine\Core\Manager\HistoryManager.h"

enum class ENGINESTATE : unsigned char
{
	PLAY,
	PAUSE,
	STOP
};

class CoreManager : public Singleton<CoreManager>
{
	ENGINESTATE currState = ENGINESTATE::STOP;
	bool isRunning = true;
public:
	CoreManager();
	~CoreManager() = default;

	void Init();
	void Update();
	void Exit();

	
	
	ENGINESTATE getEngineState();
	void setEngineState(ENGINESTATE);
private:
	int32_t scr_width{ 1280 }, scr_height{ 800 };
	
};