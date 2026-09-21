/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include "Engine/Render/Shaders/RShader.hpp"
/// | ------------------------------------ |
#include <cstdint>
/// | ------------------------------------ |

namespace ENG
{
  /**
   * @brief Entry of a render list: what to draw, on which layer and with
   *        which shader.
   *
   * RenderEntry is a lightweight record that identifies an object to be
   * drawn together with the data needed to order it (its layer) and to
   * choose how it is drawn (its shader). It does not hold any drawing data
   * itself.
   */
  struct RenderEntry
  {
    /// @brief Identifier of the object to draw.
    uint32_t id;

    /// @brief Layer the object is drawn on.
    uint8_t layer;

    /// @brief Shader used to draw the object. Not owned by the entry.
    Shader* shader;
  };
}
