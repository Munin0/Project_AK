// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Map/TileMap/TileMap.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Utils/RawGeometry.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------

namespace ENG
{
  /**
   * @brief Checks whether an object collides with a tile map (isometric).
   *
   * Samples the footprint (solid bounding box) of @p object against the
   * occupied cells of @p map. A cell is occupied when the layer holds a tile
   * at that position (for example, a wall on the "World" layer).
   *
   * @param[in,out] object     Object to test. If @p resolution is true and a
   *                           collision is found, it is moved back to its
   *                           previous position.
   * @param[in]     map        Tile map to test against.
   * @param[in]     resolution true to resolve the collision, false to only
   *                           detect it.
   * @return true if the object collides with an occupied cell, false
   *         otherwise.
   */
  [[maybe_unused]] bool CollisionTileMapIsometric(Object& object, const TileMap& map, bool resolution);

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
   * @param[in] TileMap, Collisions map.
   * @param[in] Entity ICircleBox.
   * @return true if has a collision and resolver, false otherwise.
   */
  [[maybe_unused]] bool CollisionTileToCircle(const TileMap& map, const ICircleBox& circleBox);

  [[maybe_unused]] bool CollisionRectTriangle(const ENG::Rectangle& rect, const ENG::Triangle& triangle);

}
