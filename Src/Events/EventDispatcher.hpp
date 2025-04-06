/**********************************************************************************
* \file

* \brief

* \author		Jeroen Tan Yi Xu

* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*				or disclosure of this file or its contents without the prior
*				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include <memory>
#include "EventObserver.h"

namespace Eos
{
	template <typename EventType>
	inline EventDispatcher::IndexType
	EventDispatcher::Subscribe(const std::function<void(EventType&)>& _callbackFunction)
	{
		EventObservers& observers = m_dispatchers[typeid(EventType).name()];
		const IndexType& size = observers.size();

		// Creates a new observer with _callbackFunction to be triggered
		observers.emplace_back(std::make_unique<EventObserver<EventType>>(size, _callbackFunction));
		return size;
	}

	template <typename EventType>
	inline void EventDispatcher::Unsubscribe(const IndexType _eventIndex)
	{
		// If there are no listeners attached to dispatcher, return
		EventObservers& observers = m_dispatchers[typeid(EventType).name()];
		if (observers.empty())
		{
			//do logging
			return;
		}
		//assert(observers.empty() && "No observers to unsubscrible");

		// Remove listener from list and set to point to nullptr
		observers.erase(std::remove(observers.begin(), observers.end(), observers[_eventIndex]), observers.end());
	}

	template<typename EventType>
	inline void EventDispatcher::Publish(EventType& _event)
	{
		// Get observers that are observing events of that type
		EventObservers& observers = m_dispatchers[typeid(EventType).name()];
		if (observers.empty())
		{
			//do logging
			return;
		}
		//assert(!observers.empty() && "No observers are subscribed for this event!");

		// Execute all observers that are stored in that type index in map
		for (auto& obs : observers)
		{
			if (obs == nullptr)
				continue;

			obs->Execute(_event);
		}
	}
}
