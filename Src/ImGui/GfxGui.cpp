/**********************************************************************************
* \file

* \brief

* \author		Jeroen Tan Yi Xu

* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*				or disclosure of this file or its contents without the prior
*				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "GfxGui.h"
#include "imgui.h"

ImGuiManager::ImGuiManager() :
	m_fullScreen{ false },
	m_window(nullptr),
	context(nullptr),
	m_dockspaceID(0),
	m_mainDock(0),
	dockspace_size(ImVec2(0, 0))
{
}

ImGuiManager::~ImGuiManager()
{
}

void ImGuiManager::Init(GLFWwindow* window)
{
	this->m_window = window;
	const char* glsl_version = "#version 460";

	IMGUI_CHECKVERSION();
	context = ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	//get the correct window from context
	ImGui_ImplGlfw_InitForOpenGL(Eos::Context::GetInstance().GetWindow(), true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	ImGui::StyleColorsDark();



}

void ImGuiManager::SetUpDockSpace()
{
}

ImGuiIO& ImGuiManager::GetIO()
{
	return ImGui::GetIO();
}

void ImGuiManager::BeginFrame()
{

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	bool show_demo_window = true;
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);





}

void ImGuiManager::EndFrame()
{
	ImGui::Render();
	glfwGetFramebufferSize(Eos::Context::GetInstance().GetWindow(), &Eos::Context::GetInstance().scr_width, &Eos::Context::GetInstance().scr_height);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::Destroy()
{

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

}
