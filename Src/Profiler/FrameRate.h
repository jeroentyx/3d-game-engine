/**********************************************************************************
* \file

* \brief

* \author		Jeroen Tan Yi Xu

* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*				or disclosure of this file or its contents without the prior
*				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "..\pch.h"
#include <iostream>
#include "Tools\Singleton.h"
#include "Tools\Timer.h"
#include "Tools\FixedTimer.h"

namespace Eos
{
    class FrameRateManager :
        public Singleton<FrameRateManager>
    {
    public:
        void Init();
        void Start();
        void Stop();

        double GetTime() const;
        double GetFrameRate() const;
        double delta();
        void Update();
        FixedTimer::deltaTimeType GetFixedDeltaTime() const;
        FixedTimer::stepsType GetFixedDeltaTimeSteps() const;
    private:
        Timer m_timer;
        FixedTimer m_fixedTimer;
        float lastFrame = 0.0f;
        float m_CorrectDeltaTime = 0.0f;
        double m_deltaTime = 0.0;
    };
}