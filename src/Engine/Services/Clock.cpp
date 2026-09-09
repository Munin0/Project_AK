// | -------------------------------
#include "Clock.hpp"
// | -------------------------------
#include <algorithm>
#include <chrono>
// | -------------------------------

namespace ENG
{
  void ClockManager::Init()
  {
    m_lastFrame = std::chrono::steady_clock::now();
    m_rawDeltaTime = 0.16666f;
    m_deltaTime     = 0.0f;
    m_totalTime     = 0.0f;
  }

  void ClockManager::Tick()
  {
    // Update DeltaTime
    m_actualFrame = std::chrono::steady_clock::now();
    m_deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(m_actualFrame - m_lastFrame).count();
    m_lastFrame = m_actualFrame;
    m_deltaTime = std::min(m_deltaTime, 0.05f);
    m_totalTime += m_deltaTime;
  }
  
  float ClockManager::GetDT() const
  {
    return this->m_deltaTime;
  }

  float ClockManager::GetTime() const
  {
    return this->m_totalTime;
  }

}
