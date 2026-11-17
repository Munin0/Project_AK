/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include "Engine/Utils/Vector2.hpp"
/// | ------------------------------------ |
#include <utility>
#include <vector>
/// | ------------------------------------ |

namespace ENG
{

  struct CubicBezier
  {
    Vector2 pointA, pointB;
    Vector2 controlA, controlB;
  };

  class Bezier
  {
    public:
      void AddPath(const CubicBezier& cb);
      void PopLast(void);

      void Draw(void) const;
    private:
      int m_countElements = 0;
      std::vector< std::pair<CubicBezier, int>> m_pathing;
  };
}
