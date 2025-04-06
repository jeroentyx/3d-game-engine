#include "DebugDataWindow.h"


namespace Eos 
{
	DebugDataWindow::DebugDataWindow() 
	{}

	void DebugDataWindow::Update() 
	{
		ImGui::Begin("Debug Data", &active, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
		
		timeCount++;
		if (timeCount >= TIMER) 
		{
			fps = static_cast<float>(Eos::FrameRateManager::GetInstance().GetFrameRate());
			temp = SystemManager::GetInstance().systemProfile;
			timeCount = 0;
		}
		std::string FPSstring = "FPS: ";
		FPSstring += std::to_string(fps);
		ImGui::Text(FPSstring.c_str());
		for (auto const& currSys : temp)
		{
			//ImGui::Text("%s time = %.2f%c ", get<0>(currSys), get<2>(currSys), '%');
			std::string systemString = SystemManager::GetInstance().SYSNAMEtoString(get<0>(currSys));
			systemString += " : ";
			systemString += std::to_string(get<2>(currSys));
			systemString += "ms";
			ImGui::Text(systemString.c_str());
			ImGui::ProgressBar(static_cast<float>(get<1>(currSys) / 100.f));
		}

		ImGui::End();
	}
}