/**********************************************************************************
* \file

* \brief

* \author		Jeroen Tan Yi Xu

* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*				or disclosure of this file or its contents without the prior
*				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef _IEVENT_H_
#define _IEVENT_H_

namespace Eos
{
	struct IEvent
	{
		/**********************************************************************************
		* \func		virtual ~IEvent() = default

		* \brief	Virtual default destructor.
		**********************************************************************************/
		virtual ~IEvent() = default;
	};
}

#endif
