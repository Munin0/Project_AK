// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Map/CollisionGrid.hpp"
#include "Engine/Render/Color/Color.hpp"
// | -------------------------------

namespace ENG
{
  /**
   * @brief Debug helper: draws the outline of every solid cell of a CollisionGrid.
   *
   * Works for orthogonal and isometric grids (it uses CollisionGrid::GridToWorld),
   * in world coordinates, so call it while the scene camera is active.
   *
   * @code
   * if (m_showCollision)
   *   ENG::DrawCollisionGrid(m_map.GetCollision());
   * @endcode
   *
   * @param[in] grid  Grid to draw.
   * @param[in] color Outline color (semi-transparent red by default).
   */
  void DrawCollisionGrid(const CollisionGrid& grid,
                         const Color& color = Color(1.0f, 0.2f, 0.2f, 0.8f));
}
