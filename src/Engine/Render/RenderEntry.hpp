/// | ------------------------------------ |
#pragma once
/// | ------------------------------------ |
#include "Engine/Object/ObjectPool.hpp"
#include <cstdint>
/// | ------------------------------------ |

namespace ENG
{
  static bool sortedQueue = false;

  struct RenderEntry
  {
    ObjectID id;
    uint8_t layer;
  };
}
