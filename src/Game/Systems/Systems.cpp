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
    pPos.direction.Normalize();

    pPos.position += pPos.direction * pPos.velocity * dt;
  }
}
