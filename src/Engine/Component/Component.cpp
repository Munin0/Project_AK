#include "Component.hpp"

#include "Engine/Render/RColor.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Utils/Rects.hpp"
#include "Engine/Utils/Vector2.hpp"

#include <bits/types/timer_t.h>
#include <string>
#include <vector>

namespace ENG
{
  ITransform::ITransform()
    : IComponents{}, position(0.0f,0.0f), velocity(0.0f,0.0f), angle(0.0f)
  {}

  ITransform::ITransform(const Vector2& pos, const Vector2& vel, float ang)
    : IComponents{}, position(pos), velocity(vel), angle(ang)
  {}
 
  ISprite::ISprite(const std::string& _keyName, float scale)
    : IComponents{}, keyName(_keyName), scale(scale)
  {
    image = Services::Assets().GetTexture(keyName);
    width = Services::Assets().GetTexture(keyName)->GetWidth(); 
    height = Services::Assets().GetTexture(keyName)->GetHeight();;
  }
  
  IAnimator::IAnimator(std::string key,int frames, float speed, int step, float scale)
    : IComponents{}, aName(key), frames(frames), speed(speed), step(step), scale(scale)
  {}
        
  IAnimator::IAnimator(std::string key, std::vector<ENG::Rect> _rects, float speed, int step, float scale)
    : IComponents{}, aName(key), rectangles{_rects}, speed(speed), step(step), scale(scale)
  {}

  IBoundingBox::IBoundingBox(const Vector2& dim)
    : IComponents{}, size(dim)
  {}

  IColor::IColor(const Color& c)
    : IComponents{}, color(c)
  {}

  IColor::IColor(float r, float g, float b, float a)
    : IComponents(), color(r,g,b,a)
  {}

  void IColor::ChangeColor(const Color& _color)
  {
    this->color = _color;
  }

  void IColor::ChangeColor(float r, float g, float b, float a)
  {
    this->color = Color(r,g,b,a);
  }
}
