/**********************************************************************************
* \file			

* \brief		

* \author		Jeroen Tan Yi Xu

* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*				or disclosure of this file or its contents without the prior
*				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/


#pragma once

#include <chrono>

namespace Eos
{
	class Timer
	{
		using time_point = std::chrono::time_point<std::chrono::steady_clock>;
		using clock = std::chrono::high_resolution_clock;
	public:
		Timer();
		~Timer() = default;
		void Start();
		void End();
		double GetSeconds() const;
		double GetMilliSeconds() const;
		double GetMicroSeconds() const;
	private:
		time_point m_startTime;
		time_point m_endTime;
		bool is_running;

	};
}