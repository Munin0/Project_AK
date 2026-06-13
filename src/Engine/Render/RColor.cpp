/// | ------------------------------------ |
#include "RColor.hpp"
/// | ------------------------------------ |
#include <algorithm>
#include <cstdint>
/// | ------------------------------------ |

namespace ENG 
{ 
  void Color::From255(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a)
  {
    auto clamp = [](int value) -> float
    {
      return std::clamp(value, 0, 255);
    };

    this->r = (float) clamp(_r) / 255;
    this->g = (float) clamp(_g) / 255;
    this->b = (float) clamp(_b) / 255;
    this->a = (float) clamp(_a) / 255;
  }

  const Color Color::Red    {1.0f, 0.0f,  0.0f,  1.0f};
  const Color Color::Yellow {1.0f, 1.0f,  0.0f,  1.0f};
  const Color Color::Green  {0.0f, 1.0f,  0.0f,  1.0f};
  const Color Color::Blue   {0.0f, 0.0f,  1.0f,  1.0f};
  const Color Color::White  {1.0f, 1.0f,  1.0f,  1.0f};
  const Color Color::Black  {0.0f, 0.0f,  0.0f,  1.0f};
  const Color Color::Gray   {0.3f, 0.3f, 0.3f,   1.0f};
}
