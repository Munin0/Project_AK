// | -------------------------------
#include "Component.hpp"
// | -------------------------------
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Render/Shaders/RShader.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Rects.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <string>
#include <vector>
// | -------------------------------

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

  ISprite::ISprite(const std::string& atlasKey, const std::string& animName, int frameIndex, float scale)
    : IComponents{}, keyName(atlasKey), width(0), height(0), scale(scale), isAtlas(true), animName(animName)
  {
    const AtlasData* atlas = Services::Assets().GetAtlas(atlasKey);
    if(!atlas)
    {
      LOG_ERROR("ISprite: atlas not found: " + atlasKey);
      return;
    }

    uv = GetFrameUV(*atlas, animName, frameIndex);
    atlasLayer = atlas->atlasLayer;
    width  = atlas->tileSize;
    height = atlas->tileSize;
  }

  ISprite::ISprite(const std::string& atlasKey, const std::string& tileID, float scale)
    : IComponents{}, keyName(atlasKey), width(0), height(0), scale(scale), isAtlas(true), tileID(tileID)
  {
    const AtlasData* atlas = Services::Assets().GetAtlas(atlasKey);
    if(!atlas)
    {
      LOG_ERROR("ISprite: atlas not found: " + atlasKey);
      return;
    }

    uv = GetTileUV(*atlas, tileID);
    atlasLayer = atlas->atlasLayer;
    width  = atlas->tileSize;
    height = atlas->tileSize;
  }

  void ISprite::SetFrame(int frameIndex)
  {
    if(!isAtlas)
      return;

    const AtlasData* atlas = Services::Assets().GetAtlas(keyName);
    if(!atlas)
      return;

    uv = GetFrameUV(*atlas, animName, frameIndex);
  }

  IAnimator::IAnimator(std::string key,int frames, float speed, int step, float scale)
    : IComponents{}, aName(key), frames(frames), speed(speed), step(step), scale(scale)
  {}
        
  IAnimator::IAnimator(std::string key, std::vector<ENG::Rect> _rects, float speed, int step, float scale)
    : IComponents{}, aName(key), rectangles{_rects}, speed(speed), step(step), scale(scale)
  {}

  void IAnimator::Play(void)
  {
    playing = true;
    currentFrame = 0;
    elapsed = 0.0f;
  }

  void IAnimator::Stop(void)
  {
    playing = false;
    currentFrame = 0;
    elapsed = 0.0f;
  }

  void IAnimator::Pause(void)
  {
    playing = false;
  }

  void IAnimator::Resume(void)
  {
    playing = true;
  }

  void IAnimator::Advance(float dt)
  {
    if(!playing || frames <= 0 || speed <= 0.0f)
      return;

    float frameDuration = 1.0f / speed;
    elapsed += dt;

    while(elapsed >= frameDuration)
    {
      elapsed -= frameDuration;
      currentFrame = (currentFrame + step) % frames;
    }
  }

  IBoundingBox::IBoundingBox(const Vector2& dim, bool isTrigger)
    : IComponents{}, size(dim), isTrigger(isTrigger)
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

  IMaterial::IMaterial(const std::string& idKey)
  {
    this->shader = Services::Shaders().Get(idKey);
  }

  Shader* IMaterial::GetShader() const
  {
    return this->shader;
  }

  IText::IText()
    : IComponents(), m_text(""), m_fontSize(0.0f), m_font(nullptr)
  {}
}
