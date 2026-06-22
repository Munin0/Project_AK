/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include <cstdint>
/// | ------------------------------------ |

namespace ENG
{
  class Color
  {
    public:
      constexpr Color()
        : r(1), g(1), b(1), a(1) {};

      constexpr Color(float r, float g, float b, float a = 1)
        : r(r), g(g), b(b), a(a) {};

      void From255(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a = 255);

      bool operator==(const Color& c) const
      {
        return (this->r == c.r &&
            this->g == c.g &&
            this->b == c.b && 
            this->a == c.a);
      }

      void operator=(const Color& _color)
      {
        this->r = _color.r;
        this->g = _color.g;
        this->b = _color.b;
        this->a = _color.a;
      }

    public:
      float r;
      float g;
      float b;
      float a;
    public:
      static const Color Red;
      static const Color Green;
      static const Color Yellow;
      static const Color Blue;
      static const Color White;
      static const Color Black;
      static const Color Gray;
    };
}
