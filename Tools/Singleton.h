/**********************************************************************************
* \file

* \brief

* \author		Jeroen Tan Yi Xu

* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*				or disclosure of this file or its contents without the prior
*				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#ifndef _SINGLETON_H_
#define _SINGLETON_H_


template <typename DerivedClassType>
class Singleton
{
	// Private copy and move functions to prevent copying of Singleton class
	Singleton(const DerivedClassType& _copy) = delete;
	DerivedClassType& operator=(const DerivedClassType& _copy) = delete;

	Singleton(DerivedClassType&& _move) = delete;
	DerivedClassType& operator=(DerivedClassType&& _move) = delete;

protected:
	// Private constructor to prevent construction of Singleton class
	Singleton() = default;
	virtual ~Singleton() = default;

public:
	// Gets the initialised instance of the object
	static DerivedClassType& GetInstance();
};

#include "Singleton.hpp"
#endif
