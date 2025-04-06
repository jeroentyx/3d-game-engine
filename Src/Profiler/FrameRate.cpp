#include "FrameRate.h"

void Eos::FrameRateManager::Init()
{
    m_timer.Start();
    m_deltaTime = 0.0;
}

void Eos::FrameRateManager::Start()
{
    // Get delta time
    m_timer.End();
    m_deltaTime = m_timer.GetSeconds();
    m_timer.Start();

    // udpate fixed delta time using previous frame delta time
    m_fixedTimer.CalculateTimeSteps(m_deltaTime);
}

void Eos::FrameRateManager::Stop()
{
    m_timer.End();
}

double Eos::FrameRateManager::GetTime() const
{
    return m_timer.GetSeconds();
}

double Eos::FrameRateManager::GetFrameRate() const
{
    return 1.0 / (m_deltaTime == 0 ? 0.001 : m_deltaTime);
}

double Eos::FrameRateManager::delta()
{
    return m_CorrectDeltaTime;
}

void Eos::FrameRateManager::Update()
{
    static double lastTime = glfwGetTime();
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - lastTime);
    m_CorrectDeltaTime = deltaTime;
    lastTime = currentTime;
}

Eos::FixedTimer::deltaTimeType Eos::FrameRateManager::GetFixedDeltaTime() const
{
    return static_cast<Eos::FixedTimer::deltaTimeType>(m_fixedTimer.GetFixedDeltaTime());
}

Eos::FixedTimer::stepsType Eos::FrameRateManager::GetFixedDeltaTimeSteps() const
{
    return static_cast<Eos::FixedTimer::stepsType>(m_fixedTimer.GetCurrentTimeSteps());
}
