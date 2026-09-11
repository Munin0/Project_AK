// | -------------------------------
#pragma once
// | -------------------------------
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------

namespace ENG
{
  typedef struct _rectangle
  {
    float x, y, w, h;
  }Rectangle;

  typedef struct _circle
  {
    Vector2 center;
    float radius;
  }Circle;

  typedef struct _triangle
  {
    Vector2 a,b,c;
    Vector2 center;
  }Triangle;
}
