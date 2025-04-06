#pragma once

#include "IImGuiWindow.h"
#include "..\pch.h"

#include "Src\Events\Events.h"
#include "Src\Events\EventDispatcher.h"
#include "Src\Events\EventObserver.h"
#include <thread>
#include <functional>
#include <chrono>
#include "Src\Inputs\Inputs.h"
#include "Src\Engine\Core\Manager\ComponentManager.h"
#include "Src\Engine\Core\Manager\SystemManager.h"
#include "Src\Engine\ComponentSystemID.h"
#include "Src\Engine\Systems\Graphics\Graphics_System.h"
#include "ImGuizmo\ImGuizmo.h"


#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\quaternion.hpp>
#include <glm\gtx\matrix_decompose.hpp>



namespace Eos
{
	class GameScreenWindow : public IImGuiWindow
	{
		bool m_cursor = true;
	public:
		int m_GizmoType;
		glm::mat4 m_mdl_matrix;
		EntityID m_selectedEntity;
		GameScreenWindow();
		virtual void Update() override;
		unsigned int colorBuffer{ 0 };
		std::shared_ptr<ISystem> renderingSystem;
		bool SetMousePickingOn;
	};
}