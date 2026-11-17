/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include "Engine/Render/RenderSpace.hpp"
#include "Engine/Render/Shaders/Shader.hpp"
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
    uint32_t m_id;

    /// @brief Layer the object is drawn on.
    uint8_t m_layer;

    /// @brief World (moves with the camera) or Screen (fixed on screen).
    RenderSpace m_space;

    /// @brief Shader used to draw the object. Not owned by the entry.
    Shader* m_shader;
  };
}
