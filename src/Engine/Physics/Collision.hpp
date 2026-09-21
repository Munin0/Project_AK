// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Utils/RawGeometry.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------

namespace ENG
{
  /**
   * @brief Checks whether two objects collide using their AABBs, and can
   *        optionally resolve the collision.
   *
   * AABB stands for axis-aligned bounding box.
   *
   * @param[in,out] source     Object that is tested. If @p resolution is true,
   *                           it may be modified to resolve the collision.
   * @param[in,out] target     Object that @p source is tested against.
   * @param[in]     resolution true to resolve the collision when it happens,
   *                           false to only detect it.
   * @return true if the objects collide, false otherwise.
   */
  [[maybe_unused]] bool CollisionAABB(Object& source, Object& target, bool resolution);

  /**
   * @brief Checks whether two bounding boxes overlap.
   *
   * Only detects the collision: it does not modify the boxes.
   *
   * @param[in] source First bounding box.
   * @param[in] target Second bounding box.
   * @return true if the bounding boxes overlap, false otherwise.
   */
  [[maybe_unused]] bool CollisionAABB(const IBoundingBox& source, const IBoundingBox& target);

  /**
   * @brief Checks whether a point is inside a rectangle.
   *
   * @param[in] point Point to test.
   * @param[in] rect  Rectangle: position { x, y } and size { w, h }.
   * @return true if the point is inside the rectangle, false otherwise.
   */
  [[maybe_unused]] bool CollisionPointRect(const Vector2& point, const Rectangle& rect);

  /**
   * @brief Checks whether two circles overlap.
   *
   * @param[in] a First circle.
   * @param[in] b Second circle.
   * @return true if the circles overlap, false otherwise.
   */
  [[maybe_unused]] bool CollisionCircle(const Circle& a, const Circle& b);

  /**
   * @brief Checks whether two rectangles overlap.
   *
   * @param[in] a First rectangle: position { x, y } and size { w, h }.
   * @param[in] b Second rectangle: position { x, y } and size { w, h }.
   * @return true if the rectangles overlap, false otherwise.
   */
  [[maybe_unused]] bool RectIntersects(const Rectangle& a, const Rectangle& b);
}
