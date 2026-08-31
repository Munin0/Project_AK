// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Object/ObjectPool.hpp"
#include "Engine/Utils/Rects.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <vector>
// | -------------------------------

namespace ENG
{
  struct TriggerEvent
  {
    ObjectID a;
    ObjectID b;
  };

  std::vector<TriggerEvent> ResolveCollisions(ObjectPool& pool);

  [[maybe_unused]] bool CollisionPointRect(const Vector2& point, const Rect& rect);
  [[maybe_unused]] bool RectIntersects(const Rect& a, const Rect& b);
}
