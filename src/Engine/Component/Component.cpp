// | -------------------------------
#include "Component.hpp"
// | -------------------------------
#include "Engine/Render/Color/Color.hpp"
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Render/Shaders/Shader.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/RawGeometry.hpp"
#include "Engine/Utils/Vector2.hpp"
// | -------------------------------
#include <cmath>
#include <string>
#include <utility>
#include <vector>
// | -------------------------------

namespace ENG
{
  ITransform::ITransform()
    : IComponent{}, m_position(0.0f,0.0f), m_velocity(0.0f,0.0f), m_angle(0.0f)
  {}

  ITransform::ITransform(const Vector2& pos, const Vector2& vel, float ang)
    : IComponent{}, m_position(pos), m_velocity(vel), m_angle(ang)
  {}
 
  ISprite::ISprite(const std::string& keyName, float scale)
    : IComponent{}, m_keyTexture(keyName), m_scale(scale)
  {
    auto sp = Services::Assets().GetTexture(m_keyTexture);
    if(!sp)
    {
      LOG_ERROR("ISprite: texture not found: " + keyName);
      return;
    }
    m_image = sp;
    m_width = sp->GetWidth();
    m_height = sp->GetHeight();
  }

  ISprite::ISprite(const std::string& atlasKey, const std::string& animName, int frameIndex, float scale)
    : IComponent{}, m_keyTexture(atlasKey), m_scale(scale), m_isAtlas(true), m_animationKey{animName}
  {
    const AtlasData* atlas = Services::Assets().GetAtlas(atlasKey);
    if(!atlas)
    {
      LOG_ERROR("ISprite: atlas not found: " + atlasKey);
      return;
    }

    // GetFrameUV() uses .at() internally: check the animation exists first.
    if(!atlas->m_animations.contains(animName))
    {
      LOG_ERROR("ISprite: animation not found: " + animName + " (atlas: " + atlasKey + ")");
      return;
    }

    m_uv         = GetFrameUV(*atlas, animName, frameIndex);
    m_atlasLayer = atlas->m_atlasLayer;
    m_width      = static_cast<int>(m_uv.m_pixelSize.x);
    m_height     = static_cast<int>(m_uv.m_pixelSize.y);
  }

  ISprite::ISprite(const std::string& atlasKey, const std::string& tileID, float scale)
    : IComponent{}, m_keyTexture(atlasKey), m_scale(scale), m_isAtlas(true), m_tileID(tileID)
  {
    const AtlasData* atlas = Services::Assets().GetAtlas(atlasKey);
    if(!atlas)
    {
      LOG_ERROR("ISprite: atlas not found: " + atlasKey);
      return;
    }

    if(!atlas->m_tiles.contains(tileID))
    {
      LOG_ERROR("ISprite: tile not found: " + tileID + " (atlas: " + atlasKey + ")");
      return;
    }

    // GetTileUV(): check the tile exists first.
    m_uv         = GetTileUV(*atlas, tileID);
    m_atlasLayer = atlas->m_atlasLayer;
    m_width      = static_cast<int>(m_uv.m_pixelSize.x);
    m_height     = static_cast<int>(m_uv.m_pixelSize.y);
  }

  void ISprite::SetFrame(const std::string& animKey, int frameIndex)
  {
    if(!m_isAtlas)
      return;

    const AtlasData* atlas = Services::Assets().GetAtlas(m_keyTexture);
    if(!atlas)
      return;

    if(!atlas->m_animations.contains(animKey))
    {
      LOG_ERROR("ISprite: animation not found: " + animKey + " (atlas: " + m_keyTexture + ")");
      return;
    }

    m_animationKey = animKey;
    m_uv     = GetFrameUV(*atlas, animKey, frameIndex);
    m_width  = static_cast<int>(m_uv.m_pixelSize.x);
    m_height = static_cast<int>(m_uv.m_pixelSize.y);
  }

  IAnimator::IAnimator(const std::string& atlas, const std::string& key,  float speed, int step, float scale)
    : IComponent{}, m_keyAtlas{atlas}, m_step{step}, m_speed{speed}, m_scale{scale}
  {
    SetAnimation(key);
  }

  IAnimator::IAnimator(const std::string& atlas,const std::string& key, std::vector<ENG::Rectangle> _rects, float speed, int step, float scale)
    : IComponent{}, m_keyAnimation{key}, m_keyAtlas{atlas}, m_rectangles{std::move(_rects)},
      m_frames{static_cast<int>(m_rectangles.size())}, m_step{step}, m_speed{speed}, m_scale{scale}
  { }

  bool IAnimator::SetAnimation(const std::string& keyAnim)
  {
    if(keyAnim == m_keyAnimation && m_frames > 0)
      return true; // Already playing this animation: keep the current frame.

    m_keyAnimation = keyAnim;
    m_frames       = 0;
    m_currentFrame = 0;
    m_elapsed      = 0.0f;

    const AtlasData* atlas = Services::Assets().GetAtlas(m_keyAtlas);
    if(!atlas)
    {
      LOG_ERROR("IAnimator: atlas not found: " + m_keyAtlas);
      return false;
    }

    auto it = atlas->m_animations.find(keyAnim);
    if(it == atlas->m_animations.end())
    {
      LOG_ERROR("IAnimator: animation not found: " + keyAnim + " (atlas: " + m_keyAtlas + ")");
      return false;
    }

    m_frames = it->second.m_frameCount;
    return true;
  }

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
    if(!m_playing || m_frames <= 0 || m_speed <= 0.0f) return;

    const float frameDuration = 1.0f / m_speed;
    m_elapsed += dt;

    while(m_elapsed >= frameDuration)
    {
      m_elapsed -= frameDuration;
      // Wrap into [0, m_frames) also when m_step is negative (plays backwards).
      m_currentFrame = ((m_currentFrame + m_step) % m_frames + m_frames) % m_frames;
    }
  }

  IBoundingBox::IBoundingBox(const Vector2& dim, bool isTrigger)
    : IComponent{}, m_position(0.0f, 0.0f), m_size(dim), m_isTrigger(isTrigger)
  {}

  ICircleBox::ICircleBox(float radius, bool isTrigger)
    : IComponent{}, m_position(0.0f, 0.0f), m_radius(radius), m_isTrigger(isTrigger)
  {}

  IColor::IColor(const Color& c)
    : IComponent{}, m_color{c}
  {}

  IColor::IColor(float r, float g, float b, float a)
    : IComponent{}, m_color{r,g,b,a}
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
    : IComponent{}, m_key(idKey)
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
    : IComponent(), m_text(text), m_fontSize(0.0f), m_font(nullptr)
  {}
}
