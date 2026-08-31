// | -------------------------------
#include "Object.hpp"
// | -------------------------------
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Vector2.hpp"
#include "Engine/Component/Component.hpp"
#include "Engine/Services/WorldSaver.hpp"
// | -------------------------------
#include <glm/common.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_float2.hpp>
// | -------------------------------
#include <memory>
#include <string>
// | -------------------------------

namespace ENG
{
  int Object::countObject = 0;

  Object::Object(const std::string& _name)
  {
    name        = _name;
    LOG_INFO(" | << Object: " + name);
    Object::countObject ++;
  }
  
  void Object::Draw(Batcher& b) const
  {
    auto* sprite = this->GetComponent<ISprite>();
    if(!sprite)
      return;
    glm::vec2 pos = this->GetPosition();
    glm::vec2 size = this->GetSize() * sprite->GetScale();
    glm::vec4 color = {1.0f,1.0f,1.0f,1.0f};

    if(HasComponent<IColor>())
    {
      auto c = GetComponent<IColor>();
      color = {c->GetColor().r,c->GetColor().g,c->GetColor().b,c->GetColor().a};
    }

    if(HasComponent<IMaterial>())
    {
      auto m = GetComponent<IMaterial>();
      b.SetMaterial(m->GetShader());
    }

    if(sprite->IsAtlas())
    {
      const UVRect& uv = sprite->GetUV();
      b.DrawAtlasSprite(pos, size, sprite->GetAtlasLayer(), uv.uvMin, uv.uvMax, color);
    }
    else
    {
      b.DrawTexture(pos, size, sprite->GetImage(), color);
    }
  }

  void Object::Update(float dt)
  {
    for (auto& bb : boundingBoxes)
      bb.Update(this->GetTransform().position);

    if(auto* anim = this->GetComponent<IAnimator>())
    {
      anim->Advance(dt);
      if(auto* sprite = this->GetComponent<ISprite>())
        sprite->SetFrame(anim->GetCurrentFrame());
    }
  }

  void Object::SetPosition(float x, float y)
  {
    transform.prev_position = transform.position;
    transform.position.x = x;
    transform.position.y = y;
  }

  void Object::SetPosition(const Vector2& new_pos)
  {
    this->transform.position = new_pos;
  }

  const Vector2& Object::GetPosition(void) const
  {
    return this->transform.position;
  }

  Vector2 Object::GetSize() const
  {
    auto* s = GetComponent<ISprite>();
    return {(float)s->GetWidth(), (float)s->GetHeight()};
  }
  
  /// Serialization
  ObjectState Object::Save() 
  {
    ObjectState state;
    state.m_name = this->name;
    state.m_layer = this->layer;

    // ITransform
    auto& t = this->GetTransform();
    state.m_tData = {
      .m_position = t.position,
      .m_velocity = t.velocity,
      .m_direction = t.direction,
      .m_angle = t.angle
    };

    // IStats
    auto& s = GetStats();
    state.m_sData = {
      .m_hp = s.hp,
      .m_hp_max = s.hp_max,
      .m_str = s.str,
      .m_def = s.def,
      .m_agi = s.agi
    };

    // ISprite
    if (auto *sp = GetComponent<ISprite>())
      state.m_spData = {.m_keyName = sp->GetKeyName(), .m_scale = sp->GetScale()};

    // IAnimator
    if (auto *an = GetComponent<IAnimator>())
      state.m_anData = {.m_key = an->GetName(),
                       .m_frames = an->GetFrames(),
                       .m_step = an->GetStep(),
                       .m_speed = an->GetSpeed(),
                       .m_scale = an->GetScale()};

    // IBoundingBox(es)
    for (const auto& bb : boundingBoxes)
      state.m_bbData.push_back({
        .m_width = bb.GetSize().x,
        .m_height = bb.GetSize().y,
        .m_isTrigger = bb.IsTrigger(),
      });

    // IColor
    if (auto *c = GetComponent<IColor>()) {
      auto &col = c->GetColor();
      state.m_cData = {col.r, col.g, col.b, col.a};
    }

    return state;
  }

  void Object::Load(const ObjectState& state)
  {
    this->name  = state.m_name;
    this->layer = state.m_layer;

    auto& t = GetTransform();
    t.position  = state.m_tData.m_position;
    t.velocity  = state.m_tData.m_velocity;
    t.direction = state.m_tData.m_direction;
    t.angle     = state.m_tData.m_angle;

    auto& s = GetStats();
    s.hp     = state.m_sData.m_hp;
    s.hp_max = state.m_sData.m_hp_max;
    s.str    = state.m_sData.m_str;
    s.def    = state.m_sData.m_def;
    s.agi    = state.m_sData.m_agi;

    if(state.m_spData)
    {
      AddComponent<ISprite>(state.m_spData->m_keyName, state.m_spData->m_scale);
    }

    if(state.m_anData)
    {
      AddComponent<IAnimator>(
          state.m_anData->m_key,
          state.m_anData->m_frames,
          state.m_anData->m_speed,
          state.m_anData->m_step,
          state.m_anData->m_scale
          );
    }

    for (const auto& bbData : state.m_bbData)
    {
      AddBoundingBox({bbData.m_width, bbData.m_height}, bbData.m_isTrigger);
    }

    if(state.m_cData)
    {
      AddComponent<IColor>(
          state.m_cData->m_r,
          state.m_cData->m_g,
          state.m_cData->m_b,
          state.m_cData->m_a
          );
    }
  }
}
