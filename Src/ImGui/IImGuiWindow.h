/**********************************************************************************
* \file

* \brief

* \author		Jeroen Tan Yi Xu

* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*				or disclosure of this file or its contents without the prior
*				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "glm\glm.hpp"
#include <imgui.h>
#include <string>

namespace Eos
{
	class IImGuiWindow
	{
	protected:
		glm::vec2 m_size, m_position;
		bool active, interacting;
	public:
		IImGuiWindow();
		bool GetActive();
		void SetActive(bool _active);
		glm::vec2 GetSize() const;
		glm::vec2 GetPosition() const;
		bool GetInteracting() const;

		virtual ~IImGuiWindow() = default;
		virtual void Update() = 0;
	};
}