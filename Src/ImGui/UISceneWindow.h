#pragma once


//Added from GameScreenWindow without much thinking

#include "..\pch.h"
#include "IImGuiWindow.h"
#include "Src\Events\Events.h"
#include "Src\Events\EventDispatcher.h"
#include "Src\Events\EventObserver.h"
#include "Src\Inputs\Inputs.h"
#include "Src\Engine\Core\Manager\ComponentManager.h"
#include "Src\Engine\Core\Manager\SystemManager.h"
#include "Src\Engine\ComponentSystemID.h"
#include "Src\Engine\Systems\Graphics\Graphics_System.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\quaternion.hpp>
#include <glm\gtx\matrix_decompose.hpp>
namespace Eos
{
	class UISceneWindow : public IImGuiWindow
	{
	public:
		UISceneWindow();
		~UISceneWindow();
		virtual void Update() override;
		unsigned int UISceneBuffer;
		std::shared_ptr<ISystem> renderingSystem;
	};
}