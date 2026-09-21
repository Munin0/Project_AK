/// | ------------------------------------ |
#include "Systems.hpp"
/// | ------------------------------------ |
#include "Engine/Object/Object.hpp"
/// | ------------------------------------ |

namespace APP
{
  void System_PlayerMovement(ENG::Object& o, float dt)
  {
    auto& pPos = o.GetTransform();
    pPos.m_direction.Normalize();

    pPos.m_position += pPos.m_direction * pPos.m_velocity * dt;
  }
}
