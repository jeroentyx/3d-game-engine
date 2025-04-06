/**********************************************************************************
* \file

* \brief

* \author		Jeroen Tan Yi Xu

* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*				or disclosure of this file or its contents without the prior
*				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#ifndef _BASE_OBSERVER_H_
#define _BASE_OBSERVER_H_

namespace Eos
{
	struct IEvent;

	class BaseObserver
	{
	public:
		using ObserverIndex = size_t;
		ObserverIndex m_id;

		BaseObserver(const ObserverIndex _id);

		virtual ~BaseObserver() = default;

		virtual void Execute(IEvent& _event) = 0;
		friend bool operator==(const BaseObserver& _lhs, const BaseObserver& _rhs);
	};

	bool operator==(const BaseObserver& _lhs, const BaseObserver& _rhs);
}

#endif
