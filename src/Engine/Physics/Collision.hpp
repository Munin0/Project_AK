// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Object/ObjectPool.hpp"
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

  // Naive O(n^2) broad-phase over every object pair's IBoundingBox(es). Solid-vs-solid overlaps
  // are resolved immediately: the first object of the pair (by pool iteration order) is stopped
  // at the boundary along the axis of least penetration; the second is treated as immovable and
  // never moved. Trigger overlaps (either box's IsTrigger() is true) are left untouched and
  // reported back so game code can react to them (pickups, damage zones, ...).
  std::vector<TriggerEvent> ResolveCollisions(ObjectPool& pool);
}
