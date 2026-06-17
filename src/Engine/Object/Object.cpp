// | -------------------------------
#include "Object.hpp"
// | -------------------------------
#include "Engine/Render/RColor.hpp"
#include "Engine/Services/WorldSaver.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Render/RBatch.hpp"
#include "Engine/Utils/Vector2.hpp"
#include "Engine/Component/Component.hpp"
// | -------------------------------
#include <glm/common.hpp>
#include <glm/ext/vector_float2.hpp>
// | -------------------------------
#include <glm/ext/vector_float4.hpp>
#include <memory>
#include <string>
#include <xcb/xproto.h>
// | -------------------------------

namespace ENG
{
  int Object::countObject = 0;

  Object::Object(const std::string& _name)
  {
    name        = _name + " Entity: "+ std::to_string(Object::countObject + 1);
    LOG_INFO(" | << Object: " + name);
    Object::countObject ++;
  }
  
  void Object::Draw(Batcher& b) const
  {
    if(!this->GetComponent<ISprite>())
      return;
    glm::vec2 pos = {this->GetPosition().x, this->GetPosition().y};
    glm::vec2 size = {this->GetSize().x * 6 , this->GetSize().y * 6};
    auto img = this->GetComponent<ISprite>()->GetImage();
    glm::vec4 color = {1.0f,1.0f,1.0f,1.0f};

    if(HasComponent<IColor>())
    {
      auto c = GetComponent<IColor>();
      color = {c->GetColor().r,c->GetColor().g,c->GetColor().b,c->GetColor().a};
    }

    pos = glm::round(pos);

    b.DrawTexture(pos, size, img, color);
  }

  void Object::Update(float dt)
  {
    (void)dt;
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
    auto s = GetComponent<ISprite>()->GetImage();
    return {(float)s->GetWidth(), (float)s->GetHeight()};
  }
  
  /// Serialization
  ObjectState Object::Save() const 
  {
    ObjectState state;
    state._name = this->name;
    state._layer = this->layer;

    // ITransform
    if (auto *t = GetComponent<ITransform>())
    {
      state._tData = {
        .position = t->position,
        .velocity = t->velocity,
        .direction = t->direction,
        .angle = t->angle
      };
    }

    // IStats
    if (auto *s = GetComponent<IStats>())
    {
      state._sData = {
        .hp = s->hp,
        .hp_max = s->hp_max,
        .str = s->str,
        .def = s->def,
        .agi = s->agi
      };
    }

    // ISprite
    if (auto *sp = GetComponent<ISprite>())
      state._spData = {.keyName = sp->GetKeyName(), .scale = sp->GetScale()};

    // IAnimator
    if (auto *an = GetComponent<IAnimator>())
      state._anData = {.key = an->GetName(),
                       .frames = an->GetFrames(),
                       .step = an->GetStep(),
                       .speed = an->GetSpeed(),
                       .scale = an->GetScale()};

    // IBoundingBox
    if (auto *bb = GetComponent<IBoundingBox>())
      state._bbData = {
        .width = bb->GetSize().x,
        .height = bb->GetSize().y,
      };

    // IColor
    if (auto *c = GetComponent<IColor>()) {
      auto &col = c->GetColor();
      state._cData = {col.r, col.g, col.b, col.a};
    }

    return state;
  }

  void Object::Load(const ObjectState& state)
  {
    this->name  = state._name;
    this->layer = state._layer;

    auto* t    = GetComponent<ITransform>();
    t->position  = state._tData.position;
    t->velocity  = state._tData.velocity;
    t->direction = state._tData.direction;
    t->angle     = state._tData.angle;

    auto* s = GetComponent<IStats>();
    s->hp     = state._sData.hp;
    s->hp_max = state._sData.hp_max;
    s->str    = state._sData.str;
    s->def    = state._sData.def;
    s->agi    = state._sData.agi;

    if(state._spData)
    {
      AddComponent<ISprite>(state._spData->keyName, state._spData->scale);
    }

    if(state._anData)
    {
      AddComponent<IAnimator>(
          state._anData->key,
          state._anData->frames,
          state._anData->speed,
          state._anData->step,
          state._anData->scale
          );
    }

    if(state._bbData)
    {
      Vector2 v = {state._bbData->width, state._bbData->height};
      AddComponent<IBoundingBox>(v);
    }

    if(state._cData)
    {
      AddComponent<IColor>(
          state._cData->r,
          state._cData->g,
          state._cData->b,
          state._cData->a
          );
    }
  }
}
