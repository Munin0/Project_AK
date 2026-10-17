/// | ------------------------------------ |
#include "RColor.hpp"
/// | ------------------------------------ |
#include <cstdint>
/// | ------------------------------------ |

namespace ENG 
{ 
  void Color::From255(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
  {
  constexpr float k = 1.0f / 255.0f;
  this->r = r * k;
  this->g = g * k;
  this->b = b * k;
  this->a = a * k;
  }

  const Color Color::Red    {1.0f, 0.0f,  0.0f,  1.0f};
  const Color Color::Yellow {1.0f, 1.0f,  0.0f,  1.0f};
  const Color Color::Green  {0.0f, 1.0f,  0.0f,  1.0f};
  const Color Color::Blue   {0.0f, 0.0f,  1.0f,  1.0f};
  const Color Color::White  {1.0f, 1.0f,  1.0f,  1.0f};
  const Color Color::Black  {0.0f, 0.0f,  0.0f,  1.0f};
  const Color Color::Gray   {0.3f, 0.3f, 0.3f,   1.0f};
  const Color Color::Blank  {0.0f, 0.0f, 0.0f,   0.0f};
}
