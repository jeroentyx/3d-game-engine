
#ifndef _EVENT_OBSERVER_H_
#define _EVENT_OBSERVER_H_

#include "BaseObserver.h"

namespace Eos
{
	/**********************************************************************************
	*\class		EventObserver
	
	*\brief		This class handles the observation of events being dispatched in 
				the engine

	*\tparam	EventType
				The type of event to observer for this observer
	**********************************************************************************/
	template <typename EventType>
	class EventObserver
		: public BaseObserver
	{
		// Callable to be called when event is observed
		using CallbackFunction = std::function<void(EventType&)>;
		CallbackFunction m_callbackFunc;

	public:
		/**********************************************************************************
		*\func		EventObserver(const ObserverIndex _id, const CallbackFunction& _callback)

		*\brief		Conversion constructor that takes in the id for this EventObserver
					object, as well as the callback function to trigger when an event
					is observed

		*\param		_id
					Index to register the observer with

		*\param     _callback
					Callable object to trigger as the callback when events are observed
		**********************************************************************************/
		EventObserver(const ObserverIndex _id, const CallbackFunction& _callback);

		/**********************************************************************************
		* \func		virtual void Execute(IEvent& _event) override

		* \brief	Overriden function to execute callback function when an event is
		            being observed

		*\param     _event
					The event object that contains data for triggering the callback
		**********************************************************************************/
		virtual void Execute(IEvent& _event) override;
	};
}

// Implementation of EventObserver's templated functions
#include "EventObserver.hpp"

#endif
