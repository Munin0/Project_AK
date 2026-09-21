// | -------------------------------
#pragma once
// | -------------------------------
#include <chrono>
// | -------------------------------

namespace ENG
{
  /**
   * @brief Frame clock of the engine.
   *
   * ClockManager measures the time between frames (delta time) and the
   * total time elapsed since it was initialized, using a monotonic clock
   * (std::chrono::steady_clock).
   *
   * Usage:
   * @code
   * clock.Init();
   * while (running)
   * {
   *   clock.Tick();
   *   float dt = clock.GetDT();
   *   // update with dt...
   * }
   * @endcode
   */
  class ClockManager
  {
    public:
      /**
       * @brief Initializes the clock.
       *
       * Must be called once before the first Tick().
       */
      void Init();
      /**
       * @brief Resets the clock.
       */
      void Clear();
      /**
       * @brief Advances the clock by one frame.
       *
       * Must be called once per frame. It updates the delta time and the
       * total time returned by GetDT() and GetTime().
       */
      void Tick();
      /**
       * @brief Returns the delta time.
       *
       * @return Time elapsed between the last two calls to Tick(), in seconds.
       */
      float GetDT() const;
      /**
       * @brief Returns the total elapsed time.
       *
       * @return Time elapsed since the clock was initialized, in seconds.
       */
      float GetTime() const;
    private:
      /// @brief Raw delta time of the last frame, before any adjustment.
      float m_rawDeltaTime;
      /// @brief Time point of the previous frame.
      std::chrono::steady_clock::time_point m_lastFrame;
      /// @brief Time point of the current frame.
      std::chrono::steady_clock::time_point m_actualFrame;
      /// @brief Auxiliary counter holding the time of the previous frame.
      float m_lastCounter;
      /// @brief Delta time returned by GetDT(), in seconds.
      float m_deltaTime;
      /// @brief Total elapsed time returned by GetTime(), in seconds.
      float m_totalTime;
  };
}
