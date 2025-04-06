/**********************************************************************************
* \file

* \brief

* \author		Jeroen Tan Yi Xu

* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*				or disclosure of this file or its contents without the prior
*				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "IImGuiWindow.h"

namespace Eos
{
	IImGuiWindow::IImGuiWindow() :
		m_size(glm::vec2(0, 0)),
		m_position(glm::vec2(0, 0)),
		active(true),
		interacting(false)
	{
	}
	bool IImGuiWindow::GetActive()
	{
		return active;
	}
	void IImGuiWindow::SetActive(bool _active)
	{
		active = _active;
	}
	glm::vec2 IImGuiWindow::GetSize() const
	{
		return m_size;
	}
	glm::vec2 IImGuiWindow::GetPosition() const
	{
		return m_position;
	}
	bool IImGuiWindow::GetInteracting() const
	{
		return interacting;
	}
}