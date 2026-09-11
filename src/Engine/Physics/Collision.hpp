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
  [[maybe_unused]] bool CollisionAABB(Object& source, Object& target, bool resolution);
  [[maybe_unused]] bool CollisionAABB(const IBoundingBox& source, const IBoundingBox& target);
  [[maybe_unused]] bool CollisionPointRect(const Vector2& point, const Rectangle& rect);
  [[maybe_unused]] bool CollisionCircle(const Circle& a, const Circle& b);
  [[maybe_unused]] bool RectIntersects(const Rectangle& a, const Rectangle& b);
}
