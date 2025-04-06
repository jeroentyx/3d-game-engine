#include "DepthWindow.h"


namespace Eos
{
	DepthWindow::DepthWindow()
	{
	}
	void DepthWindow::Update()
	{
		ImGui::Begin("Tools", &active, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
		
		std::shared_ptr<ISystem> renderingSystem = SystemManager::GetInstance().getSystem(SYSNAME::SYS_GRAPHICS);
		std::shared_ptr<Rendering_System>  rs = std::dynamic_pointer_cast<Rendering_System>(renderingSystem);
		ImGui::Text("Change Light Position ");
		ImGui::DragFloat3("Light Pos", &rs->lightPos.x, 0.01f);

		ImGui::End();
	}

}
