/// | ------------------------------------ |
#include "Systems.hpp"
/// | ------------------------------------ |
#include "Engine/Map/CollisionGrid.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Physics/Collision.hpp"
#include "Engine/Utils/Vector2.hpp"
/// | ------------------------------------ |

namespace APP
{
  void System_PlayerMovement(ENG::Object& o, float dt)
  {
    auto& pPos = o.GetTransform();
    pPos.m_direction.Normalize();

    pPos.m_position += pPos.m_direction * pPos.m_velocity * dt;
    pPos.m_direction = {0,0};
  }

  void System_MoveOnGrid(ENG::Object& o, ENG::Vector2 dir, float speed, const ENG::CollisionGrid& grid, float dt)
  {
    if (dir.x == 0.0f && dir.y == 0.0f)
      return;

    dir.Normalize();
    const ENG::Vector2 next = o.GetPosition() + dir * (speed * dt);
    // SetPosition(x, y) stores the previous position, which the resolution needs.
    o.SetPosition(next.x, next.y);
    ENG::CollisionObjectGrid(o, grid, true);
  }
}
