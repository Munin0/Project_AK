#include "Component.hpp"

#include "Engine/Services/Services.hpp"
#include "Engine/Utils/Vector2.hpp"

#include <bits/types/timer_t.h>
#include <string>

namespace ENG
{
  IName::IName(const std::string& n)
    : IComponents{}, name{n} 
  {}

  ITransform::ITransform()
    : IComponents{},
        position(0.0f,0.0f),
        velocity(0.0f,0.0f),
        angle(0.0f)
  {}

  ITransform::ITransform(const Vector2& pos, const Vector2& vel, float ang)
    : IComponents{},
        position(pos),
        velocity(vel),
        angle(ang)
  {}

  ITimer::ITimer()
    : IComponents{}, actualFrame{0}
  {}
 
  ISprite::ISprite(const std::string& _keyName, float scale)
    : IComponents{}, keyName(_keyName), scale(scale)
  {
    image = Services::Assets().GetTexture(keyName);
    width = Services::Assets().GetTexture(keyName)->GetWidth(); 
    height = Services::Assets().GetTexture(keyName)->GetHeight();;
  }
    
  IShader::IShader(const std::string& vert, const std::string& frag)
    : IComponents{}, m_vertshaderFile(vert), m_fragshaderFile(frag)
  {}

  IControl::IControl()
    : IComponents{}
  {}

  IBoundingBox::IBoundingBox(const Vector2& dim)
    : IComponents{}
  {}

}
