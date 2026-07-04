/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include "Engine/Render/Shaders/RShader.hpp"
/// | ------------------------------------ |
#include <cstdint>
/// | ------------------------------------ |

namespace ENG
{
  struct RenderEntry
  {
    uint32_t id;
    uint8_t layer;
    Shader* shader;
  };
}
