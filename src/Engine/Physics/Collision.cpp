// | -------------------------------
#include "Collision.hpp"
// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Utils/RawGeometry.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include "glm/geometric.hpp"
// | -------------------------------

namespace ENG
{
  bool CollisionAABB(Object& source, Object& target, bool resolution)
  {
    Vector2 posA = source.GetPosition(), posB = target.GetPosition();
    auto dimA = source.GetSolidBox()->GetSize();
    auto dimB = target.GetSolidBox()->GetSize();
    bool r = (posA.x <= dimB.x && dimA.x >= posB.x) && (posA.y <= dimB.y && dimA.y >= posB.y);

    if(r && resolution)
    {
      auto p = source.GetTransform();
      p.position = p .prev_position;
    }

    return r;
  }

  bool CollisionAABB(const IBoundingBox& source, const IBoundingBox& target)
  {
    Vector2 posA = source.GetPosition(), posB = target.GetPosition();
    auto dimA = source.GetSize();
    auto dimB = target.GetSize();
    return (posA.x <= dimB.x && dimA.x >= posB.x) && (posA.y <= dimB.y && dimA.y >= posB.y);
  }

  bool CollisionPointRect(const Vector2 &point, const Rectangle &rect)
  {
    return (point.x >= rect.x && point.x <= rect.x + rect.w) && (point.y >= rect.y && point.y <= rect.y + rect.h); 
  }

  bool CollisionCircle(const Circle& a, const Circle& b)
  {
    auto distSq = glm::dot(b.center.x - a.center.x, b.center.y - a.center.y);
    float radiusSum = a.radius + b.radius;
    return distSq <= (radiusSum * radiusSum);
  }

  bool RectIntersects(const Rectangle& a, const Rectangle& b)
  {
    return !(a.w < b.x || b.w < a.x || a.h < b.y || b.h < a.y);
  }
}
