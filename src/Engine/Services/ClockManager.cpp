// | -------------------------------
#include "ClockManager.hpp"
// | -------------------------------
#include <algorithm>
#include <chrono>
// | -------------------------------

namespace ENG
{
  void ClockManager::Init()
  {
    m_lastFrame = std::chrono::steady_clock::now();
    m_rawDeltaTime = 0.016666f;
    m_deltaTime     = 0.0f;
    m_totalTime     = 0.0f;

    m_fps = 0.0f;
    m_fpsAccum = 0.0f;
    m_fpsFrames = 0;
  }

  void ClockManager::Clear()
  {
    m_rawDeltaTime  = 0.16666f;
    m_lastCounter   = 0.0f;
    m_deltaTime     = 0.0f;
    m_totalTime     = 0.0f;

    m_fps = 0.0f;
    m_fpsAccum = 0.0f;
    m_fpsFrames = 0;
  }

  void ClockManager::Tick()
  {
    // Update DeltaTime && Time
    m_actualFrame = std::chrono::steady_clock::now();
    m_deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(m_actualFrame - m_lastFrame).count();
    m_rawDeltaTime = std::chrono::duration<float>(m_actualFrame - m_lastFrame).count();
    m_lastFrame = m_actualFrame;
    m_deltaTime = std::min(m_deltaTime, 0.05f);
    m_totalTime += m_deltaTime;

    /// Update FPS 
    m_fpsAccum += m_rawDeltaTime;
    ++m_fpsFrames;
    if (m_fpsAccum >= 0.5f)
    {
      m_fps       = m_fpsFrames / m_fpsAccum;
      m_fpsAccum  = 0.f;
      m_fpsFrames = 0;
    }
  }

  float ClockManager::GetFPS() const
  {
    return m_fps;
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
