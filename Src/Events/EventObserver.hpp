/**********************************************************************************
* \file

* \brief

* \author		Jeroen Tan Yi Xu

* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*				or disclosure of this file or its contents without the prior
*				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "IEvent.h"

namespace Eos
{
	template <typename EventType>
	inline EventObserver<EventType>::EventObserver(const ObserverIndex _id, const CallbackFunction& _callback)
		: BaseObserver{ _id },
		  m_callbackFunc{ _callback }
	{
	}

	template <typename EventType>
	inline void EventObserver<EventType>::Execute(IEvent& _event)
	{
		m_callbackFunc(*(static_cast<EventType*>(&_event)));
	}
}
