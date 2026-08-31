// | -------------------------------
#include "Collision.hpp"
// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Object/ObjectPool.hpp"
#include "Engine/Utils/Rects.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <algorithm>
#include <cstddef>
#include <vector>
// | -------------------------------

namespace ENG
{
  bool Overlap(const IBoundingBox& a, const IBoundingBox& b, Vector2& outDepth)
  {
    float aLeft = a.GetPosition().x, aRight = aLeft + a.GetSize().x;
    float bLeft = b.GetPosition().x, bRight = bLeft + b.GetSize().x;
    float overlapX = std::min(aRight, bRight) - std::max(aLeft, bLeft);
    if (overlapX <= 0.0f)
      return false;

    float aBottom = a.GetPosition().y, aTop = aBottom + a.GetSize().y;
    float bBottom = b.GetPosition().y, bTop = bBottom + b.GetSize().y;
    float overlapY = std::min(aTop, bTop) - std::max(aBottom, bBottom);
    if (overlapY <= 0.0f)
      return false;

    outDepth = {overlapX, overlapY};
    return true;
  }

  std::vector<TriggerEvent> ResolveCollisions(ObjectPool& pool)
  {
    std::vector<TriggerEvent> triggers;
    const auto& ids = pool.GetAllIDs();

    for (size_t i = 0; i < ids.size(); i++)
    {
      Object* objA = pool.Get(ids[i]);
      if (!objA || !objA->HasBoundingBoxes())
        continue;

      for (size_t j = i + 1; j < ids.size(); j++)
      {
        Object* objB = pool.Get(ids[j]);
        if (!objB || !objB->HasBoundingBoxes())
          continue;

        for (auto& boxA : objA->GetBoundingBoxes())
        {
          for (auto& boxB : objB->GetBoundingBoxes())
          {
            Vector2 depth;
            if (!Overlap(boxA, boxB, depth))
              continue;

            if (boxA.IsTrigger() || boxB.IsTrigger())
            {
              triggers.push_back({ids[i], ids[j]});
              continue;
            }

            Vector2 posA = objA->GetTransform().position;
            Vector2 centerA = boxA.GetCenter();
            Vector2 centerB = boxB.GetCenter();

            if (depth.x < depth.y)
            {
              float sign = (centerA.x < centerB.x) ? -1.0f : 1.0f;
              posA.x += sign * depth.x;
            }
            else
            {
              float sign = (centerA.y < centerB.y) ? -1.0f : 1.0f;
              posA.y += sign * depth.y;
            }

            objA->SetPosition(posA);

            for (auto& bb : objA->GetBoundingBoxes())
              bb.Update(posA);
          }
        }
      }
    }

    return triggers;
  }


  bool CollisionPointRect(const Vector2 &point, const Rect &rect)
  {
    return (point.x >= rect.x && point.x <= rect.x + rect.w) && (point.y >= rect.y && point.y <= rect.y + rect.h); 
  }

  bool RectIntersects(const Rect& a, const Rect& b)
  {
    return !(a.w < b.x || b.w < a.x || a.h < b.y || b.h < a.y);
  }
}
