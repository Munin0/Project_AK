// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------

namespace ENG
{
  /**
   * @brief Raw rectangle, used only for storage.
   *
   * Holds the position and the size of a rectangle. It has no behavior.
   *
   * - Position: { x, y }
   * - Size:     { w, h }
   */
  struct Rectangle
  {
    float x;  ///< X coordinate of the rectangle's position.
    float y;  ///< Y coordinate of the rectangle's position.
    float w;  ///< Width of the rectangle.
    float h;  ///< Height of the rectangle.
  };
  /**
   * @brief Raw circle, used only for storage.
   *
   * Holds the center and the radius of a circle. It has no behavior.
   */
  struct Circle
  {
    Vector2 center;  ///< Center of the circle: { x, y }.
    float radius;    ///< Radius of the circle.
  };
  /**
   * @brief Raw triangle, used only for storage.
   *
   * Holds the three vertices of a triangle and its center. It has no
   * behavior: the center is not computed automatically, so it is the
   * responsibility of the caller to keep it consistent with the vertices.
   */
  struct Triangle
  {
    Vector2 a;       ///< First vertex: { x, y }.
    Vector2 b;       ///< Second vertex: { x, y }.
    Vector2 c;       ///< Third vertex: { x, y }.
    Vector2 center;  ///< Center of the triangle: { x, y }.
  };
}
