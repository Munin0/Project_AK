// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Map/CollisionGrid.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Utils/RawGeometry.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------

namespace ENG
{
  /**
   * @brief Checks whether an object collides with the solid cells of a map.
   *
   * Samples the footprint (the lower part of the sprite, or of the bounding
   * box if there is no sprite) of @p object against the solid cells of
   * @p grid. Works for orthogonal and isometric grids.
   *
   * The resolution moves the object back towards
   * ITransform::m_prev_position, so move it with Object::SetPosition(x, y)
   * (which stores the previous position) before calling this.
   *
   * @param[in,out] object     Object to test. If @p resolution is true and a
   *                           collision is found, it is moved back to its
   *                           previous position.
   * @param[in]     grid       Collision grid to test against.
   * @param[in]     resolution true to resolve the collision, false to only
   *                           detect it.
   * @return true if the object collides with an occupied cell, false
   *         otherwise.
   */
  [[maybe_unused]] bool CollisionObjectGrid(Object& object, const CollisionGrid& grid, bool resolution);

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
   * @brief Checks whether two rectangle's overlap.
   *
   * Only detects the collision: it does not modify the rects.
   *
   * @param[in] source First rectangle.
   * @param[in] target Second rectangle.
   * @return true if the rectangle's overlap, false otherwise.
   */
  [[maybe_unused]] bool CollisionAABB(const Rectangle& a, const Rectangle& b);

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
   * @brief Checks A Tilemap of Collisions vs a Entity->ICircleBox.
   *
   * @param[in] grid Collision grid.
   * @param[in] Entity ICircleBox.
   * @return true if has a collision and resolver, false otherwise.
   */
  [[maybe_unused]] bool CollisionTileToCircle(const CollisionGrid& grid, const ICircleBox& circleBox);

  [[maybe_unused]] bool CollisionRectTriangle(const ENG::Rectangle& rect, const ENG::Triangle& triangle);

}
