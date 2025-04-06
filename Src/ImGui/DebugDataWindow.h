#pragma once

#include "..\pch.h"
#include "IImGuiWindow.h"
#include "../Src/Engine/ComponentSystemID.h"
#include "../Engine/Core/Manager/SystemManager.h"
#include <vector>

#define TIMER 100

namespace Eos 
{
	class DebugDataWindow :public IImGuiWindow
	{
		int timeCount = TIMER;
		float fps = 0;
		std::vector<std::tuple<SYSNAME, double, double>> temp;
	public:
		DebugDataWindow();
		virtual void Update() override;
	};
}
