/**********************************************************************************
* \file

* \brief

* \author		Jeroen Tan Yi Xu

* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*				or disclosure of this file or its contents without the prior
*				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "..\Tools\Timer.h"
namespace Eos
{
	Timer::Timer() :is_running{ false }
	{
	}
	void Timer::Start()
	{
		is_running = true;
		m_startTime = clock::now();
	}
	void Timer::End()
	{
		m_endTime = clock::now();
		is_running = false;
	}
	double Timer::GetSeconds() const
	{
		time_point endTime;
		if (is_running)
		{
			endTime = clock::now();
		}
		else
		{
			endTime = m_endTime;
		}
		return std::chrono::duration<double>(endTime - m_startTime).count();
	}



	double Timer::GetMilliSeconds() const
	{
		return GetSeconds() * 1000;
	}
	double Timer::GetMicroSeconds() const
	{
		return GetSeconds() * 1000000;
	}
}