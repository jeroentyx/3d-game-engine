#pragma once

#include "..\pch.h"
#include "IImGuiWindow.h"
#include "Src\Engine\Core\Manager\ComponentManager.h"
#include "Src\Engine\Core\Manager\SystemManager.h"
#include "Src\Engine\ComponentSystemID.h"
#include "Src\Engine\Systems\Graphics\Graphics_System.h"


namespace Eos
{
	class DepthWindow :
		public IImGuiWindow
	{

	public:
		DepthWindow();
		virtual void Update() override;
	};
}
