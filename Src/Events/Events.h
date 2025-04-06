/**********************************************************************************
* \file

* \brief

* \author		Jeroen Tan Yi Xu

* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*				or disclosure of this file or its contents without the prior
*				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef _EVENTS_H_
#define _EVENTS_H_

#include "IEvent.h"
#include <unordered_set>
#include <forward_list>
#include <string>
#include <memory>
#include "..\pch.h"
#include "..\ImGui\IImGuiWindow.h"
#include <glm\glm.hpp>



namespace Eos
{
	// Entity Events --------------------------------------------------------------------
	struct SendSelectionIDEvent : IEvent
	{
		EntityID selectedEntity;
		SendSelectionIDEvent(EntityID const _value) : selectedEntity{ _value } { }
	};


	struct SendFrameBufferObjectEvent : IEvent
	{
		unsigned int textureColorBuffer;
		SendFrameBufferObjectEvent(unsigned int _value) : textureColorBuffer{ _value } { }
	};

	struct ResizeFrameBufferEvent : IEvent
	{
		glm::vec2 position, size;
		ResizeFrameBufferEvent(const glm::vec2& _pos, const glm::vec2& _size)
			: position{ _pos }, size{ _size } {}
	};

	struct hoveringEvent : IEvent
	{
		bool isHovered, enableEditorView;

		hoveringEvent(bool hover, bool enable)
			:isHovered(hover), enableEditorView(enable) {}
	};


	struct SetPickingEvent : IEvent
	{
		bool togglePicking;

		SetPickingEvent(bool flag)
			:togglePicking(flag){}
	};

	struct SetAnimationPlayingEvent : IEvent
	{
		std::string meshName;

		SetAnimationPlayingEvent(std::string name)
			:meshName{name}{}
	};
}

#endif
