/// | ------------------------------------ |
#include "Interpolation.hpp"
/// | ------------------------------------ |
#include "Vector2.hpp"
/// | ------------------------------------ |
/// | ------------------------------------ |

namespace ENG
{
  Interpolation& Interpolation::Get() const
  {
    if(!m_instance)
    {
      m_instance = new Interpolation();
    }
    return *m_instance;
  }

  Vector2 Interpolation::Lerp(const Vector2& init, const Vector2& dest, float t)
  {
    float tm = (1-t);
    return Vector2{ 
      tm * init.x + ( dest.x - init.x ),
      tm * init.y + ( dest.y - init.y )
    };
  }

  Vector2 Interpolation::LerpTwo(const Vector2& init, const Vector2& dest, const Vector2& crtl, float t)
  {
    float tm = (1-t);
    float tm2 = tm * tm;
    float t2 = t*t;
    return Vector2{
      tm2 * init.x + (2*tm*t*crtl.x) + t2*dest.x,
      tm2 * init.y + (2*tm*t*crtl.y) + t2*dest.y
    };
  }

  Vector2 Interpolation::LerpThree(const Vector2& init, const Vector2& dest, const Vector2& crtl1, const Vector2& crtl2, float t)
  {
    float tm = (1-t);
    float tm2 = tm * tm;
    float tm3 = tm*tm*tm;
    float t2 = t*t;
    float t3 = t*t*t;

    return Vector2{
    };
  }

  Vector2 Interpolation::Linear(const Vector2& start, const Vector2& dest, float factor)
  {
    return this->Lerp(start, dest, factor);
  }

  Vector2 Interpolation::Linear_EaseIn(const Vector2& start, const Vector2& dest, float factor)
  {
    auto t = factor*factor;
    return this->Lerp(start, dest, t);
  }

  Vector2 Interpolation::Linear_EaseOut(const Vector2& start, const Vector2& dest, float factor)
  {
    auto t = (factor * (2 - factor));
    return this->Lerp(start, dest, t);
  }

  Vector2 Interpolation::Linear_InOut(const Vector2& start, const Vector2& dest, float factor)
  {
    auto t = factor < 0.5 ? 2*factor*factor : -1+(4-2*factor)*factor;
    return this->Lerp(start, dest, t);
  }

  Vector2 Interpolation::Linear_SmoothStep(const Vector2& start, const Vector2& dest, float factor)
  {
    auto t = factor * factor * (3 - 2 * factor);
    return this->Lerp(start, dest, t);
  }

  Vector2 Interpolation::Linear_DeltaTimeStep(const Vector2& speed, float dt)
  {
    auto f = speed * dt;


    return f;
  }
}
