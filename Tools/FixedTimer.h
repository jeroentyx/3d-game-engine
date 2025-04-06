#pragma once

namespace Eos
{
	class FixedTimer
	{
	public:
		using stepsType																		= size_t;
		using deltaTimeType																	= double;

		FixedTimer();
		void										CalculateTimeSteps(const deltaTimeType _deltaTime);
		stepsType									GetCurrentTimeSteps() const;
		static deltaTimeType						GetFixedDeltaTime();

	private:
		// Constant expression variables
		static constexpr deltaTimeType fixedDeltaTime									 = 1.0 / 60.0;

		deltaTimeType m_accumulatedTime; // Counter that accummulates the delta time passed since last fixed update
		stepsType m_currTimeSteps;       // current number of time steps
	};

}