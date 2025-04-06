#pragma once

#include "IImGuiWindow.h"

namespace Eos
{
	class UIWindow : public IImGuiWindow
	{
	public:
		UIWindow();
		virtual void Update() override;
	};
}