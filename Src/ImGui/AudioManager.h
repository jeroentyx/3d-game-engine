#pragma once

#include "..\pch.h"
#include "IImGuiWindow.h"
#include "../Src/Engine/ComponentSystemID.h"
#include "../Engine/Core/Manager/SystemManager.h"
#include "../Engine/Systems/Audio/Audio_System.h"
#include <vector>

namespace Eos
{
	class AudioManager :public IImGuiWindow
	{
		std::vector<std::string> BGMList;
		std::vector<std::string> SFXList;
		int selectedBGMindex;
		int selectedSFXindex;
		bool firstLaunch;
		bool isBGMTab;
		std::string currentlyPlaying;
	public:
		AudioManager();
		virtual void Update() override;

		void ReloadAudio();
		void UpdatePlayingString(std::string);
	};
}