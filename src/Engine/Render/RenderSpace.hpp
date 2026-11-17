// | -------------------------------
#pragma once
// | -------------------------------
#include <cstdint>
// | -------------------------------

namespace ENG
{
  /**
   * @brief Space an object is drawn in.
   *
   * - World:  world coordinates; the scene camera (Camera2D) moves / zooms it.
   *           Players, enemies, props, text over a character...
   * - Screen: screen pixels; the camera is ignored, it stays fixed on screen.
   *           HUD, menus, dialog boxes, a fixed background...
   *
   * The space is independent of the layer: the layer says what is drawn on top,
   * the space says whether it moves with the camera.
   */
  enum class RenderSpace : uint8_t
  {
    World  = 0,
    Screen = 1
  };
}
