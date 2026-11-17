/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include "Vector2.hpp"
/// | ------------------------------------ |
#include <vector>
/// | ------------------------------------ |

namespace ENG
{
  class Interpolation
  {
    public:
      /**
       * @brief Returns the global Interpolation instance.
       * @return Reference to the singleton instance.
       */
      Interpolation& Get() const;
      
      Vector2 Linear(const Vector2& start, const Vector2& dest, float factor);
      Vector2 Linear_EaseIn(const Vector2& start, const Vector2& dest, float factor);
      Vector2 Linear_EaseOut(const Vector2& start, const Vector2& dest, float factor);
      Vector2 Linear_InOut(const Vector2& start, const Vector2& dest, float factor);
      Vector2 Linear_SmoothStep(const Vector2& start, const Vector2& dest, float factor);
      Vector2 Linear_DeltaTimeStep(const Vector2& speed, float dt);

      Vector2 Quadratic(const Vector2& start, const Vector2& dest, const Vector2& ctrl, float factor);

    private:
        Vector2 Lerp(const Vector2& init, const Vector2& dest, float t);
        Vector2 LerpTwo(const Vector2& init, const Vector2& dest, const Vector2& ctrl, float t);
        Vector2 LerpThree(const Vector2& init, const Vector2& dest, const Vector2& ctrl1, const Vector2& ctrl2, float t);
    private:
      /// @brief Default constructor. Private: the instance is managed through Get().
      Interpolation() = default;
      /// @brief Copying is not allowed: there is a single Interpolation instance.
      Interpolation(const Interpolation&)            = delete;
      /// @brief Assignment is not allowed: there is a single Interpolation instance.
      Interpolation& operator=(const Interpolation&) = delete;
      /// @brief Default destructor. Private: the instance is released through Destroy().
      ~Interpolation() = default;
    private:
      std::vector<Vector2> m_path;
      static inline Interpolation* m_instance = nullptr;
  };
}
