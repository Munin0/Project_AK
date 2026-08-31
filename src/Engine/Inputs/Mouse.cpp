// | -------------------------------
#include "Mouse.hpp"
// | -------------------------------
#include "SDL3/SDL_mouse.h"
// | -------------------------------
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------

namespace ENG
{
  const Vector2 GetMousePosition(void)
  {
    float x,y;
    SDL_GetMouseState(&x, &y);
    return Vector2{x,y};
  }
}
