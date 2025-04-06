#pragma once
#include "../../Core/Interfaces/ISystem.h"
#include "../../Core/Interfaces/IComponent.h"
#include "../../ComponentSystemID.h"
#include "fmod.h"
#include "fmod_common.h"
#include <map>
#include "tracy\Tracy.hpp"
#include "../Src/Engine/Engine/CoreManager.h"
#include <filesystem>

#define GET_AUDIO_SYSTEM std::dynamic_pointer_cast<Audio_System>(SystemManager::SystemManager::GetInstance().getSystem(SYSNAME::SYS_AUDIO))

class Audio_Component : public IComponent
{
	friend class Audio_System;
	virtual void Init() override;

public:
	void PlayBGM(std::string audioName);
	void PlaySFX(std::string audioName);
};

class Audio_System : public ISystem
{
	std::shared_ptr<Audio_System> instance;
	//Map to keep track of Audio Components
	std::map<EntityID, std::shared_ptr<Audio_Component>> aComps;
	//Map to keep track of AudioName and FMOD Sound*, 1 for BGMs, 1 for SFX
	std::map<std::string, FMOD_SOUND*> BGMMap;
	std::map<std::string, FMOD_SOUND*> SFXMap;

	float Mastervol = 0.0f;
	float prevMastervol = 0.0f;
	float BGvol = 0.0f;
	float SFXvol = 0.0f;

	FMOD_SYSTEM* fmod_system = nullptr;
	FMOD_CHANNEL* channelBGM = nullptr;
	FMOD_CHANNEL* channelSFX = nullptr;
	FMOD_CHANNEL* channelTest = nullptr;
public:

	//Overloaded stuff from ISystem
	void Init();
	void Update();
	void Exit();
	void AddingNewComponent(EntityID entityID, COMPONENTNAME compName, std::shared_ptr<IComponent> compPtr);
	void RemovingComponent(EntityID entityID, COMPONENTNAME compName);
	void ClearAllComponents();

	//FrontEnd Functions
	void PlayBGM(std::string key);
	void StopBGM();
	void SetBGMVol(float val);
	void IncreaseBGMVol();
	void DecreaseBGMVol();

	void PlaySFX(std::string key);
	void StopSFX();
	void SetSFXVol(float val);
	void IncreaseSFXVol();
	void DecreaseSFXVol();

	void SetMasterVol(float val);
	void IncreaseMasterVol();
	void DecreaseMasterVol();

	void Mute();
	void Unmute();

	void PauseAllAudio();
	void UnPauseAllAudio();
	void StopAllAudio();

	//Editor Functions (Seperate Channel to Sample music while system is paused)
	void PlayTest(std::string key);
	void StopTest();
	void PauseTest();
	void UnPauseTest();
	std::vector<std::string> getBGMList(); //To get the list of BGMs
	std::vector<std::string> getSFXList(); //To get the list of SFXs


	//Fmod Backend
	//Build an Audio file from filename to FMOD_Sound Ptr (Helper Func)
	FMOD_SOUND* BuildAudio(const std::string& filename, bool isBGM);
	//Input the Fmod Ptrs into their respective maps 
	void LoadAudioObjects(const std::string& filepath, bool isBGM);
	//Function to read all files in Assets library and load all audio into the system
	void ReloadAllAudio();
	//Func to get the Fmod Ptr from music map
	FMOD_SOUND* GetBGM(std::string key);
	FMOD_SOUND* GetSFX(std::string key);
};