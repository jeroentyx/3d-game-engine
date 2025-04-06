#include "SystemManager.h"

SystemManager::SystemManager()
{
	AddSystem(SYSNAME::SYS_GRAPHICS, std::make_shared<Rendering_System>());
	AddSystem(SYSNAME::SYS_PHYSICS, std::make_shared<Physics_System>());
	AddSystem(SYSNAME::SYS_LOGIC, std::make_shared<Eos::ScriptSystem>());
	AddSystem(SYSNAME::SYS_AUDIO, std::make_shared<Audio_System>());
	AddSystem(SYSNAME::SYS_UI, std::make_shared<UI_System>());
};

std::shared_ptr<ISystem> SystemManager::getSystem(SYSNAME systemName)
{
	return allSystems.at(systemName);
}

void SystemManager::AddSystem(SYSNAME systemName, std::shared_ptr<ISystem> systemPtr)
{
	if (allSystems.count(systemName) == 0)
	{
		allSystems.insert({ systemName, systemPtr });
	}
}

void SystemManager::InitAllSystems()
{
	/*Eos::Logger::GetInstance().Init();*/
	PE_CORE_INFO("System Manager Created!");
	//InitAudio();
	//LoadAudio();

	//TOFIX: 
	//for (auto const& cs : allSystems)
	//{
	//	cs.second->Init();
	//}

	////Loop all Systems->Init() here
	for (auto const& currSys : allSystems)
	{
		currSys.second->Init();
	}
}

void SystemManager::InitAllManagers()
{

}

void SystemManager::UpdateAllSystems()
{
	//TOFIX: 
	//////Loop all Systems->Update() here
	std::map<SYSNAME, clock_t> clocksPerSystem;
	clock_t start = clock();
	for (auto const& currSys : allSystems)
	{
		clock_t start1 = clock();
		if (CoreManager::GetInstance().getEngineState() == ENGINESTATE::PAUSE || CoreManager::GetInstance().getEngineState() == ENGINESTATE::STOP)
		{
			if (currSys.first == SYSNAME::SYS_GRAPHICS || currSys.first == SYSNAME::SYS_AUDIO || currSys.first == SYSNAME::SYS_UI)
			{
				currSys.second->Update();
			}
		}
		else 
		{
			currSys.second->Update();
		}
		clock_t end1 = clock();
		clocksPerSystem.insert({ currSys.first, (end1 - start1) });
	}
	clock_t end = clock();
	clock_t clocksPerLoop = end - start;
	if (clocksPerLoop <= 0)
		clocksPerLoop = 1;

	systemProfile.clear();
	for (auto const& currSys : clocksPerSystem)
	{
		systemProfile.emplace_back(currSys.first, ((static_cast<double>(currSys.second) / clocksPerLoop) * 100), (static_cast<double>(currSys.second) / CLOCKS_PER_SEC));
	}


	if (Eos::Inputs::GetInstance().IsKeyPressed(EOS_KEY_F1))
	{
		/*
		if (isSoundPlaying == false)
		{
			PlayAudio(SOUNDTYPE::ST_BackgroundMusic);
			isSoundPlaying = true;
		}
		else if (isSoundPlaying == true)
		{
			StopBGAudio();
			isSoundPlaying = false;
		}
		*/
	}
}

void SystemManager::UpdateAllManagers()
{

}

void SystemManager::ExitAllSystems()
{
	for (auto const& currSys : allSystems)
	{
		currSys.second->Exit();
	}
	Eos::Context::GetInstance().Destroy();
}

void SystemManager::ClearAllComponentsFromSystems()
{
	for (auto const& currSys : allSystems)
	{
		currSys.second->ClearAllComponents();
	}
}

std::string SystemManager::SYSNAMEtoString(SYSNAME sysName)
{
	switch (sysName)
	{
	case SYSNAME::SYS_PHYSICS:
		return std::string("Physics System");
		break;
	case SYSNAME::SYS_GRAPHICS:
		return std::string("Graphics System");
		break;
	case SYSNAME::SYS_SERIALIZER:
		return std::string("Serializer System");
		break;
	case SYSNAME::SYS_AUDIO:
		return std::string("Audio System");
		break;
	default:
		return std::string("Unknown/Unadded Component");
		break;
	}
}
