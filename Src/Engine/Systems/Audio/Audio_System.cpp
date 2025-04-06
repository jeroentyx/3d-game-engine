#include "Audio_System.h"

void Audio_System::Init()
{
	FMOD_System_Create(&fmod_system);
	FMOD_System_Init(fmod_system, 32, FMOD_INIT_NORMAL, nullptr);

	channelBGM = 0;
	channelSFX = 0;
	channelTest = 0;

	Mastervol = 1.f;
	prevMastervol = Mastervol;
	BGvol = 0.3f;
	SFXvol = 0.5f;
	ReloadAllAudio();
}

void Audio_System::Update()
{
	FMOD_Channel_SetVolume(channelBGM, (Mastervol * BGvol));
	FMOD_Channel_SetVolume(channelSFX, (Mastervol * SFXvol));

	if (CoreManager::CoreManager::GetInstance().getEngineState() == ENGINESTATE::PAUSE)
	{
		PauseAllAudio();
	}
	else if (CoreManager::CoreManager::GetInstance().getEngineState() == ENGINESTATE::PLAY)
	{
		UnPauseAllAudio();
	}
	else if (CoreManager::CoreManager::GetInstance().getEngineState() == ENGINESTATE::STOP)
	{
		StopAllAudio();
	}
	
}

void Audio_System::Exit()
{
	
}

void Audio_System::AddingNewComponent(EntityID entityID, COMPONENTNAME compName, std::shared_ptr<IComponent> compPtr)
{
	if (compName == COMPONENTNAME::Comp_Audio_Component)
	{
		if (!aComps.contains(entityID))
		{
			std::shared_ptr<Audio_Component> temp = std::dynamic_pointer_cast<Audio_Component>(compPtr);
			//temp->ASInstance = instance;
			aComps.insert({ entityID, temp });
		}
	}
}

void Audio_System::RemovingComponent(EntityID entityID, COMPONENTNAME compName)
{
	if (compName == COMPONENTNAME::Comp_Audio_Component)
	{
		aComps.erase(entityID);
	}
}

void Audio_System::ClearAllComponents()
{
	aComps.clear();
}

void Audio_System::PlayBGM(std::string key)
{
	//Only shd have 1 BGM playing at a time
	FMOD_Channel_Stop(channelBGM);
	FMOD_System_PlaySound(fmod_system, GetBGM(key), 0, false, &channelBGM);
	FMOD_Channel_SetVolume(channelBGM, (Mastervol * BGvol));
}

void Audio_System::StopBGM()
{
	FMOD_Channel_Stop(channelBGM);
}

void Audio_System::SetBGMVol(float val) 
{
	if (val > 1.f) 
	{
		val = 1;
	}
	else if (val < 0.f) 
	{
		val = 0;
	}
	BGvol = val;
}

void Audio_System::IncreaseBGMVol()
{
	BGvol += 0.1f;
	if (BGvol > 1.0f) 
	{
		BGvol = 1.0f;
	}
}

void Audio_System::DecreaseBGMVol()
{
	BGvol -= 0.1f;
	if (BGvol < 0.f)
	{
		BGvol = 0.f;
	}
}

void Audio_System::PlaySFX(std::string key)
{
	//FMOD_Channel_Stop(channelSFX);
	FMOD_System_PlaySound(fmod_system, GetSFX(key), 0, false, &channelSFX);
	FMOD_Channel_SetVolume(channelSFX, (Mastervol * SFXvol));
}

void Audio_System::StopSFX()
{
	FMOD_Channel_Stop(channelSFX);
}


void Audio_System::SetSFXVol(float val)
{
	if (val > 1.f)
	{
		val = 1;
	}
	else if (val < 0.f)
	{
		val = 0;
	}
	SFXvol = val;
}

void Audio_System::IncreaseSFXVol()
{
	SFXvol += 0.1f;
	if (SFXvol > 1.0f)
	{
		SFXvol = 1.0f;
	}
}

void Audio_System::DecreaseSFXVol()
{
	SFXvol -= 0.1f;
	if (SFXvol < 0.f)
	{
		SFXvol = 0.f;
	}
}

void Audio_System::SetMasterVol(float val)
{
	if (val > 1.f)
	{
		val = 1;
	}
	else if (val < 0.f)
	{
		val = 0;
	}
	Mastervol = val;
}

void Audio_System::IncreaseMasterVol()
{
	Mastervol += 0.1f;
	if (Mastervol > 1.0f)
	{
		Mastervol = 1.0f;
	}
}

void Audio_System::DecreaseMasterVol()
{
	Mastervol -= 0.1f;
	if (Mastervol < 0.f)
	{
		Mastervol = 0.f;
	}
}

void Audio_System::Mute()
{
	prevMastervol = Mastervol;
	Mastervol = 0.0f;
}

void Audio_System::Unmute()
{
	Mastervol = prevMastervol;
}

void Audio_System::PauseAllAudio()
{
	FMOD_Channel_SetPaused(channelBGM, true);
	FMOD_Channel_SetPaused(channelSFX, true);
}



void Audio_System::UnPauseAllAudio()
{
	FMOD_Channel_SetPaused(channelBGM, false);
	FMOD_Channel_SetPaused(channelSFX, false);
}

void Audio_System::PlayTest(std::string key)
{
	FMOD_Channel_Stop(channelTest);
	if (BGMMap.contains(key))
	{
		FMOD_System_PlaySound(fmod_system, BGMMap[key], 0, false, &channelTest);
	}
	else if (SFXMap.contains(key))
	{
		FMOD_System_PlaySound(fmod_system, SFXMap[key], 0, false, &channelTest);
	}
	FMOD_Channel_SetVolume(channelTest, Mastervol);
}

void Audio_System::StopTest() 
{
	FMOD_Channel_Stop(channelTest);
}

void Audio_System::PauseTest() 
{
	FMOD_Channel_SetPaused(channelTest, true);
}
void Audio_System::UnPauseTest() 
{
	FMOD_Channel_SetPaused(channelTest, false);
}

std::vector<std::string> Audio_System::getBGMList()
{
	std::vector<std::string> temp;
	temp.emplace_back("");
	for (auto const& [key, value] : BGMMap) 
	{
		temp.emplace_back(key);
	}
	return temp;
}

std::vector<std::string> Audio_System::getSFXList()
{
	std::vector<std::string> temp;
	temp.emplace_back("");
	for (auto const& [key, value] : SFXMap)
	{
		temp.emplace_back(key);
	}
	return temp;
}

void Audio_System::StopAllAudio()
{
	FMOD_Channel_Stop(channelBGM);
	FMOD_Channel_Stop(channelSFX);
}

FMOD_SOUND* Audio_System::BuildAudio(const std::string& filename, bool isBGM)
{
	FMOD_SOUND* sound = 0;
	if (isBGM) 
	{
		FMOD_System_CreateSound(fmod_system, filename.c_str(), FMOD_LOOP_NORMAL, FMOD_DEFAULT, &sound);
	}
	else 
	{
		FMOD_System_CreateSound(fmod_system, filename.c_str(), FMOD_LOOP_OFF, FMOD_DEFAULT, &sound);
	}
	return sound;
}



void Audio_System::LoadAudioObjects(const std::string& filepath, bool isBGM)
{
	std::filesystem::path fileName = std::filesystem::path(filepath).filename(); 
	if (isBGM) 
	{
		BGMMap.insert(std::pair<std::string, FMOD_SOUND*>(fileName.string(), BuildAudio(filepath, true)));
		//std::cout << fileName.string() << " added to BGM Library!" << std::endl;
	}
	else 
	{
		SFXMap.insert(std::pair<std::string, FMOD_SOUND*>(fileName.string(), BuildAudio(filepath, false)));
		//std::cout << fileName.string() << " added to SFX Library!" << std::endl;
	}
	
}

void Audio_System::ReloadAllAudio()
{
	BGMMap.clear();
	//std::cout << "Reloading Audio Library..." << std::endl;
	std::string BGMpath = "Assets/Audio/BGM";
	for (const auto& entry : std::filesystem::directory_iterator(BGMpath)) 
	{
		LoadAudioObjects(entry.path().string(),true);
	}
	//std::cout << "BGM Library Complete!" << std::endl;
	SFXMap.clear();
	std::string SFXpath = "Assets/Audio/SFX";
	for (const auto& entry : std::filesystem::directory_iterator(SFXpath))
	{
		LoadAudioObjects(entry.path().string(), false);
	}	
	//std::cout << "SFX Library Complete!" << std::endl;
}

FMOD_SOUND* Audio_System::GetBGM(std::string key)
{
	if (BGMMap.contains(key)) 
	{
		return BGMMap[key];
	}
	else 
	{
		//std::cout << "BGM Not Found!" << std::endl;
		return nullptr;
	}
}

FMOD_SOUND* Audio_System::GetSFX(std::string key)
{
	if (SFXMap.contains(key))
	{
		return SFXMap[key];
	}
	else
	{
		//std::cout << "SFX Not Found!" << std::endl;
		return nullptr;
	}
}

void Audio_Component::Init()
{
}

void Audio_Component::PlayBGM(std::string audioName)
{
	GET_AUDIO_SYSTEM->PlayBGM(audioName);
}

void Audio_Component::PlaySFX(std::string audioName)
{
	GET_AUDIO_SYSTEM->PlaySFX(audioName);
}
