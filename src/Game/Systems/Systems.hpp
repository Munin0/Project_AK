/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include "Engine/Map/CollisionGrid.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Utils/Vector2.hpp"
/// | ------------------------------------ |

namespace APP
{
  [[maybe_unused]] void System_PlayerMovement(ENG::Object& o, float dt);

  /**
   * @brief Moves an object in a direction and resolves it against a CollisionGrid.
   *
   * @param[in,out] o     Object to move (needs an IBoundingBox to collide).
   * @param[in]     dir   Direction; it does not need to be normalized. {0,0} = no movement.
   * @param[in]     speed Speed, in world units per second.
   * @param[in]     grid  Collision of the map.
   * @param[in]     dt    Delta time.
   */
  [[maybe_unused]] void System_MoveOnGrid(ENG::Object& o, ENG::Vector2 dir, float speed, const ENG::CollisionGrid& grid, float dt);
}
