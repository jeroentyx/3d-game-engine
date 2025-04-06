/**********************************************************************************
* \file

* \brief

* \author		Jeroen Tan Yi Xu

* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*				or disclosure of this file or its contents without the prior
*				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef HGUI_H_
#define HGUI_H_

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "Context.h"
#include "Tools\Singleton.h"

class ImGuiManager : public Singleton<ImGuiManager>
{
public:
	ImGuiManager();
	~ImGuiManager();

	void Init(GLFWwindow* window);
	void SetUpDockSpace();
	ImGuiIO& GetIO();
	void BeginFrame();
	void EndFrame();
	void Destroy();
	void Render();
private:
	bool m_fullScreen{ false };
	ImGuiContext* context;
	ImGuiID m_dockspaceID;
	ImGuiID m_mainDock;
	ImVec2 dockspace_size;
	ImVec4 clear_color = ImVec4(0.1f, 0.1f, 0.1f, 0.6f);
	GLFWwindow* m_window;
	ImFont* Font{ nullptr };
	unsigned char* pixels{ nullptr };
};

#endif