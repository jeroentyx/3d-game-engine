#pragma once

#include "..\..\ComponentSystemID.h"
#include "..\Interfaces\ISystem.h"
#include "..\..\Systems\Graphics\Graphics_System.h"
#include "..\..\Systems\Physics\Physics_System.h"
#include "..\..\Systems\Audio\Audio_System.h"
#include "..\..\Systems\UI\UI_System.h"
#include "Src\pch.h"
#include <iostream>
#include "..\Src\ImGui\Context.h"
#include "Tools\GlfwHelpers.h"
#include "Tools\Singleton.h"
#include "..\Src\ImGui\GfxGui.h"
#include "Src\Profiler\FrameRate.h"



class SystemManager : 
	public Singleton<SystemManager>
{
	//Map to manage all Systems
	std::map<SYSNAME, std::shared_ptr<ISystem>> allSystems;

public:
	SystemManager();
	SystemManager(const SystemManager&) = delete;
	SystemManager(SystemManager&&) = delete;
	SystemManager& operator=(const SystemManager&) = delete;
	SystemManager& operator=(SystemManager&&) = delete;

	std::shared_ptr<ISystem> getSystem(SYSNAME systemName);

	std::vector<std::tuple<SYSNAME, double, double>> systemProfile;
	bool isSoundPlaying = false;
	void AddSystem(SYSNAME systemName, std::shared_ptr<ISystem> systemPtr);
	void InitAllSystems();
	void InitAllManagers();
	void UpdateAllSystems();
	void UpdateAllManagers();
	void ExitAllSystems();
	void ClearAllComponentsFromSystems();
	//Stupid unelegant way to convert enum to string, update it to use ListAllEntitiesOfComponent()
	std::string SYSNAMEtoString(SYSNAME);
};