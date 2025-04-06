#include "FixedTimer.h"

namespace Eos
{
	FixedTimer::FixedTimer():
		m_accumulatedTime(0.0f),
		m_currTimeSteps((Eos::FixedTimer::stepsType)0.0f)
	{
	}
	void FixedTimer::CalculateTimeSteps(const deltaTimeType _deltaTime)
	{
		// At the start of a new frame, reset the timer to default state
		m_currTimeSteps = 0;
		m_accumulatedTime += _deltaTime;

		// In the case where the engine is to hang, calculate the number of timesteps
		// required to catch up to current state of engine
		//normal optimization
		while (m_accumulatedTime >= fixedDeltaTime)
		{
			m_accumulatedTime -= fixedDeltaTime;
			++m_currTimeSteps;
		}
	}
	FixedTimer::stepsType FixedTimer::GetCurrentTimeSteps() const
	{
		return m_currTimeSteps;
	}
	FixedTimer::deltaTimeType FixedTimer::GetFixedDeltaTime()
	{
		return fixedDeltaTime;
	}
}