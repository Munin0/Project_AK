/// | -------------------------------
#include "Text.hpp"
/// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Services/WorldSaver.hpp"
#include "Engine/Text/TextAPI.hpp"
#include "Engine/Utils/Vector2.hpp"
/// | -------------------------------
#include <string>
/// | -------------------------------

namespace ENG
{
  Text::Text(const Vector2& position,float fontSize, const std::string& text)
    : Object(text)
  {
    this->m_text.SetText(text);
    this->m_text.SetFontSize(fontSize);
    this->SetPosition(position);
  }

  void Text::Draw(Batcher& b) const
  {
    const auto font = Services::Fonts().GetFont("PottaOne"); 
    const auto color =this->GetComponent<ENG::IColor>()->GetColor();
    TextAPI::Get().DrawTextEx(*font,this->GetPosition(),this->m_text.GetText(), this->m_text.GetFontSize(), color);
  }

  void Text::Update(float dt)
  {
    (void)dt;
  }

  ObjectState Text::Save()
  {
    ObjectState state;
    state.m_name = Object::GetName();
    state.m_layer = Object::GetLayer();

    // ITransform
    auto& t = Object::GetTransform();
    state.m_tData = {
      .m_position = t.position,
      .m_velocity = t.velocity,
      .m_direction = t.direction,
      .m_angle = t.angle
    };

    // IStats
    auto& s = Object::GetStats();
    state.m_sData = {
      .m_hp = s.hp,
      .m_hp_max = s.hp_max,
      .m_str = s.str,
      .m_def = s.def,
      .m_agi = s.agi
    };

    // IText
    state.m_txData = {m_text.GetText(), m_text.GetFontSize(), m_text.GetFont()->GetName()};

    // ISprite
    if (auto* sp = GetComponent<ISprite>())
    {
      state.m_spData = {.m_keyName = sp->GetKeyName(), .m_scale = sp->GetScale()};
    }

    // IAnimator
    if (auto* an = GetComponent<IAnimator>())
    {
      state.m_anData = {.m_key = an->GetName(),
        .m_frames = an->GetFrames(),
        .m_step = an->GetStep(),
        .m_speed = an->GetSpeed(),
        .m_scale = an->GetScale()};
    }

    // IBoundingBox(es)
    for (const auto& bb : boundingBoxes)
    {
      state.m_bbData.push_back({
          .m_width = bb.GetSize().x,
          .m_height = bb.GetSize().y,
          .m_isTrigger = bb.IsTrigger(),
          });
    }

    // IColor
    if (auto* c = GetComponent<IColor>())
    {
      auto &col = c->GetColor();
      state.m_cData = {col.r, col.g, col.b, col.a};
    }

    return state;
  }

  void Text::Load(const ObjectState& state)
  {
    Object::SetName(state.m_name);
    Object::SetLayer(state.m_layer);

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

    if(state.m_txData)
    {
      auto* f = Services::Fonts().GetFont(state.m_txData->m_fontKey);
      m_text.SetText(state.m_txData->m_text);
      m_text.SetFontSize(state.m_txData->m_fontSize);
      m_text.SetFont(f);
    }
  }

}
