// | -------------------------------
#pragma once
// | -------------------------------
#include <chrono>
// | -------------------------------

namespace ENG
{
  class ClockManager
  {
    public:
      void Init();
      void Clear();
      void Tick();

      float GetDT() const;
      float GetTime() const;

    private:
    float m_rawDeltaTime;
    std::chrono::steady_clock::time_point m_lastFrame;
    std::chrono::steady_clock::time_point m_actualFrame;

    float m_lastCounter;
    float m_deltaTime;
    float m_totalTime;
  };
}
