#include "UISceneWindow.h"
#pragma once

namespace Eos
{


	UISceneWindow::UISceneWindow()
	{
		//Get the Graphics_System from System Manager
		//renderingSystem = std::dynamic_pointer_cast<Rendering_System>(SystemManager::GetInstance().getSystem(SYSNAME::SYS_GRAPHICS));

	}

	UISceneWindow::~UISceneWindow()
	{

	}

	void UISceneWindow::Update()
	{
		//Something is wrong with ImGUI
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 1,1 });
		ImGui::Begin("UISceneWindow");

		const ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		const ImVec2 vMax = ImGui::GetWindowContentRegionMax();

		ImVec2 windowPos = ImGui::GetWindowPos();
		windowPos.y += ImGui::GetWindowContentRegionMin().y;
		ImVec2 contentRegionSize = ImGui::GetContentRegionAvail();

		//TODO: 
		//renderingSystem->SetUpSceneFBO();
		//std::shared_ptr<Rendering_System>  rs = std::dynamic_pointer_cast<Rendering_System>(renderingSystem);
		//rs->SetUpSceneFBO();

		//ImGui::Image((void*)(intptr_t)rs->UISceneColorBuffer,
		//	ImVec2(m_size.x, m_size.y),
		//	ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));



		ImGui::End();
		ImGui::PopStyleVar();
	}

}