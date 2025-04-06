/**********************************************************************************
* \file

* \brief

* \author		Jeroen Tan Yi Xu

* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*				or disclosure of this file or its contents without the prior
*				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef _EVENT_DISPATCHER_H_
#define _EVENT_DISPATCHER_H_

#include "..\Tools\Singleton.h"
#include <list>
#include <functional>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <string>

namespace Eos
{
	// Forward declaration of BaseObserver class
	class BaseObserver;

	/**********************************************************************************
	*\class		EventDispatcher
	
	*\brief		This class acts like a bus and handles the interaction between the
				the event objects and the engine, by handling the storing (subscribing)
				and sending (publishing) of events around the different systems and 
				managers.
	**********************************************************************************/
	class EventDispatcher
		: public Singleton<EventDispatcher>
	{
		// Type aliases that are event related
		
		using EventIndex = std::string_view;
		using EventObservers = std::list<std::unique_ptr<BaseObserver>>;
		using EventMap = std::unordered_map<EventIndex, EventObservers>;

		// Container storing all observers that are observing a specified type of event
		EventMap m_dispatchers;

	public:
		// Type aliases of type used by observers to indicate their index numbers
		using IndexType = size_t;

		/**********************************************************************************
		*\func		IndexType Subscribe
		            (const std::function<void(EventType&)>& _callbackFunction)

		*\brief		Stores an observer that will be observing an event and triggering
		            a callback function when event is observed.

		*\tparam	EventType
					Type of event to be stored and observed

		*\param     _callbackFunction
					Callable obect that is to be triggered when EventType events are 
					observed

		*\return	Returns the index of the newly generated observer that observes
		            EventType events.
		**********************************************************************************/
		template <typename EventType>
		IndexType Subscribe(const std::function<void(EventType&)>& _callbackFunction);

		/**********************************************************************************
		*\func		void Unsubscribe(const size_t _eventIndex)

		*\brief		Removes an observer from the currently stored observers.
		
		*\tparam	EventType
					Type of event to be stored and observed

		*\param     _eventIndex
					Index of event observer to remove (unsubscribe)
		**********************************************************************************/
		template <typename EventType>
		void Unsubscribe(const IndexType _eventIndex);

		/**********************************************************************************
		*\func		void Publish(EventType* _event)

		*\brief		Executes all observers that are observing the type of the event
					object hat is to be sent out/published.

		*\tparam	EventType
					Type of event to be stored and observed

		*\param     _event
					The event object containing data that is to be sent to all observers
					that are observing this type of event
		**********************************************************************************/
		template <typename EventType>
		void Publish(EventType& _event);
	};
}

// Global type aliases
using IndexType = Eos::EventDispatcher::IndexType;


// Macros for easier usage of dispatcher
#define PE_SUBSCRIBE_EVENT(_eventType, _callback) \
	EventDispatcher::GetInstance().Subscribe<_eventType>(_callback)

#define PE_UNSUBSCRIBE_EVENT(_eventType, _index) \
	EventDispatcher::GetInstance().Unsubscribe<_eventType>(_index)

#define PE_PUBLISH_EVENT(_event) \
	EventDispatcher::GetInstance().Publish(_event)

#define BIND_MEMFUNC_CALLBACK(_memFuncClass, _memFunc, _instance) \
	std::bind(&_memFuncClass::_memFunc, _instance, std::placeholders::_1)

// Implementation of EventDispatcher's functions
#include "EventDispatcher.hpp"

#endif
