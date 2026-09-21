// | -------------------------------
#include "Component.hpp"
// | -------------------------------
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Render/Shaders/RShader.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/RawGeometry.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <string>
#include <vector>
// | -------------------------------

namespace ENG
{
  ITransform::ITransform()
    : IComponents{}, m_position(0.0f,0.0f), m_velocity(0.0f,0.0f), m_angle(0.0f)
  {}

  ITransform::ITransform(const Vector2& pos, const Vector2& vel, float ang)
    : IComponents{}, m_position(pos), m_velocity(vel), m_angle(ang)
  {}
 
  ISprite::ISprite(const std::string& keyName, float scale)
    : IComponents{}, m_keyTexture(keyName), m_scale(scale)
  {
    m_image = Services::Assets().GetTexture(m_keyTexture);
    m_width = Services::Assets().GetTexture(m_keyTexture)->GetWidth();
    m_height = Services::Assets().GetTexture(m_keyTexture)->GetHeight();;
  }

  ISprite::ISprite(const std::string& atlasKey, const std::string& animName, int frameIndex, float scale)
    : IComponents{}, m_keyTexture(atlasKey), m_width(0), m_height(0), m_scale(scale), m_isAtlas(true), m_animationKey{animName}
  {
    const AtlasData* atlas = Services::Assets().GetAtlas(atlasKey);
    if(!atlas)
    {
      LOG_ERROR("ISprite: atlas not found: " + atlasKey);
      return;
    }

    m_uv = GetFrameUV(*atlas, animName, frameIndex);
    m_atlasLayer = atlas->atlasLayer;
    m_width  = atlas->tileSize;
    m_height = atlas->tileSize;
  }

  ISprite::ISprite(const std::string& atlasKey, const std::string& tileID, float scale)
    : IComponents{}, m_keyTexture(atlasKey), m_width(0), m_height(0), m_scale(scale), m_isAtlas(true), m_tileID(tileID)
  {
    const AtlasData* atlas = Services::Assets().GetAtlas(atlasKey);
    if(!atlas)
    {
      LOG_ERROR("ISprite: atlas not found: " + atlasKey);
      return;
    }

    m_uv = GetTileUV(*atlas, tileID);
    m_atlasLayer = atlas->atlasLayer;
    m_width  = atlas->tileSize;
    m_height = atlas->tileSize;
  }

  void ISprite::SetFrame(int frameIndex)
  {
    if(!m_isAtlas)
      return;

    const AtlasData* atlas = Services::Assets().GetAtlas(m_keyTexture);
    if(!atlas)
      return;

    m_uv = GetFrameUV(*atlas, m_animationKey, frameIndex);
  }

  IAnimator::IAnimator(std::string key, int frames, float speed, int step, float scale)
    : IComponents{}, m_keyAnimation{key}, m_frames{frames}, m_speed{speed}, m_step{step}, m_scale{scale}
  {}
        
  IAnimator::IAnimator(std::string key, std::vector<ENG::Rectangle> _rects, float speed, int step, float scale)
    : IComponents{}, m_keyAnimation{key}, m_rectangles{_rects}, m_speed{speed}, m_step{step}, m_scale{scale}
  {}

  void IAnimator::Play(void)
  {
    m_playing = true;
    m_currentFrame = 0;
    m_elapsed = 0.0f;
  }

  void IAnimator::Stop(void)
  {
    m_playing = false;
    m_currentFrame = 0;
    m_elapsed = 0.0f;
  }

  void IAnimator::Pause(void)
  {
    m_playing = false;
  }

  void IAnimator::Resume(void)
  {
    m_playing = true;
  }

  void IAnimator::Advance(float dt)
  {
    if(!m_playing || m_frames <= 0 || m_speed <= 0.0f)
      return;

    float m_frameDuration = 1.0f / m_speed;
    m_elapsed += dt;

    while(m_elapsed >= m_frameDuration)
    {
      m_elapsed -= m_frameDuration;
      m_currentFrame = (m_currentFrame + m_step) % m_frames;
    }
  }

  IBoundingBox::IBoundingBox(const Vector2& dim, bool isTrigger)
    : IComponents{}, m_size(dim), m_isTrigger(isTrigger)
  {}

  IColor::IColor(const Color& c)
    : IComponents{}, m_color(c)
  {}

  IColor::IColor(float r, float g, float b, float a)
    : IComponents(), m_color(r,g,b,a)
  {}

  void IColor::ChangeColor(const Color& _color)
  {
    this->m_color = _color;
  }

  void IColor::ChangeColor(float r, float g, float b, float a)
  {
    this->m_color = Color(r,g,b,a);
  }

  IMaterial::IMaterial(const std::string& idKey)
  {
    if(auto* shader = Services::Shaders().Get(idKey))
      this->m_shader = shader;
    else
      LOG_ERROR(" | << Shader not exist: " + idKey);
  }

  Shader* IMaterial::GetShader() const
  {
    if(!m_shader)
    {
      LOG_ERROR(" | << ERROR, GetShader cant return a shader, doesnt have one");
      return nullptr;
    }
    return this->m_shader;
  }

  IText::IText(const std::string& text)
    : IComponents(), m_text(text), m_fontSize(0.0f), m_font(nullptr)
  {}
}
