#pragma once

#include "..\pch.h"
#include "IImGuiWindow.h"
#include "Src\Events\Events.h"
#include "Src\Events\EventDispatcher.h"
#include "Src\Engine\Core\Manager\ComponentManager.h"
#include "Src\Engine\Core\Manager\SystemManager.h"
#include "Src\Engine\ComponentSystemID.h"
#include "Src\Engine\Systems\Graphics\Graphics_System.h"
#include "Src\Engine\Systems\Graphics\EosRenderer\EosEditorResources.h"
#include "imfilebrowser.h"



namespace Eos
{
	class InspectorWindow :
		public IImGuiWindow
	{
		//using EntityID = int;
		EntityID m_selectedEntity;
		EntityID prevSelected;
		int item_current_idx;
		glm::vec3 force{ 0 };
		std::string m_path;
		std::vector<std::string> Scripts;

	public:
		std::shared_ptr<ISystem> renderingSystem;
		void resetSelectedEntity();
		InspectorWindow();
		virtual void Update() override;
	};
}
