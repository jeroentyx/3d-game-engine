#pragma once

#include "..\pch.h"
#include "IImGuiWindow.h"
#include "Src\Engine\Core\Manager\ComponentManager.h"
#include "Src\Engine\Core\Manager\SystemManager.h"
#include "Src\Engine\ComponentSystemID.h"
#include "Src\Engine\Systems\Graphics\Graphics_System.h"
#include "stb_image\stb_image.h"

namespace Eos
{
	class GraphicWindow :
		public IImGuiWindow
	{
		int oldSelection = -1;
		bool isDirty = false;
	public:
		std::shared_ptr<ISystem> renderingSystem;
		GraphicWindow();
		virtual void Update() override;
	};
}