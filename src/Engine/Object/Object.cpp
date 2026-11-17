// | -------------------------------
#include "Object.hpp"
// | -------------------------------
#include "Engine/Render/Color/Color.hpp"
#include "Engine/Render/Batching/Batch.hpp"
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/Vector2.hpp"
#include "Engine/Component/Component.hpp"
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Services/WorldSaver.hpp"
#include "Engine/Component/ComponentSerializers.hpp"
// | -------------------------------
#include <glm/common.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/vector_float2.hpp>
// | -------------------------------
#include <memory>
#include <string>
#include <cstdlib>
#include <typeindex>
#include <unordered_set>
#if defined(__GNUG__)
#include <cxxabi.h>
#endif
// | -------------------------------

namespace ENG
{
  int Object::s_countObject = 0;

  Object::Object(const std::string& name)
  {
    m_name        = name;
    LOG_INFO(" | << Object: " + m_name);
    Object::s_countObject ++;
  }
  
  void Object::Draw(Batcher& b) const
  {
    auto* sprite = this->GetComponent<ISprite>();
    if(!sprite)
      return;
    glm::vec2 pos = this->GetPosition();
    glm::vec2 size = this->GetSizeSprite() * sprite->GetScale();
    glm::vec4 color = {1.0f,1.0f,1.0f,1.0f};

    if(HasComponent<IColor>())
    {
      auto c = GetComponent<IColor>();
      color = {c->GetColor().m_r,c->GetColor().m_g,c->GetColor().m_b,c->GetColor().m_a};
    }

    if(HasComponent<IMaterial>())
    {
      auto m = GetComponent<IMaterial>();
      b.SetMaterial(m->GetShader());
    }

    if(sprite->IsAtlas())
    {
      const UVRect& uv = sprite->GetUV();
      glm::vec2 size = this->GetSizeSprite() * sprite->GetScale();
      b.DrawAtlasSprite(pos, size, sprite->GetAtlasLayer(), uv.m_uvMin, uv.m_uvMax, color);
    }
    else
    {
      glm::vec2 size = this->GetSizeSprite() * sprite->GetScale();
      b.DrawTexture(pos, size, sprite->GetImage(), color);
    }
  }

  void Object::Update(float dt)
  {
    if(auto* anim = this->GetComponent<IAnimator>())
    {
      anim->Advance(dt);
      // An invalid animator (missing atlas/animation) was already reported once; skip it
      // so SetFrame() does not log the same error every frame.
      if(auto* sprite = this->GetComponent<ISprite>(); sprite && anim->IsValid())
        sprite->SetFrame(anim->GetKeyAnimation(), anim->GetCurrentFrame());
    }
  }

  void Object::SetPosition(float x, float y)
  {
    m_transform.m_prev_position = m_transform.m_position;
    m_transform.m_position.x = x;
    m_transform.m_position.y = y;
    if(auto box = this->GetComponent<ENG::IBoundingBox>())
      box->Update(m_transform.m_position);
  }

  void Object::SetPosition(const Vector2& new_pos)
  {
    this->m_transform.m_position = new_pos;
    if(auto box = this->GetComponent<ENG::IBoundingBox>())
      box->Update(m_transform.m_position);
  }

  const Vector2& Object::GetPosition(void) const
  {
    return this->m_transform.m_position;
  }

  Vector2 Object::GetSizeSprite() const
  {
    if(auto s = GetComponent<ISprite>())
      return {(float)s->GetWidth(), (float)s->GetHeight()};
    return {0.0f,0.0f};
  }
  
  /// Serialization
  ObjectState Object::Save() 
  {
    ObjectState state;
    state.m_name = this->m_name;
    state.m_layer = this->m_layer;
    state.m_renderSpace = static_cast<uint8_t>(this->m_renderSpace);
    state.m_visible = this->m_visible;

    // ITransform
    auto& t = this->GetTransform();
    state.m_tData = {
      .m_position      = t.m_position,
      .m_prev_position = t.m_prev_position,
      .m_velocity   = t.m_velocity,
      .m_direction  = t.m_direction,
      .m_prev_direction = t.m_prev_direction,
      .m_angle      = t.m_angle
    };

    // ISprite
    if (auto* sp = GetComponent<ISprite>())
    {
      state.m_spData = {
        .m_keyName = sp->GetKeyName(),
        .m_width   = sp->GetWidth(),
        .m_height  = sp->GetHeight(),
        .m_scale   = sp->GetScale(),
        .m_animKey = sp->GetAnimationKey(),
        .m_tileID  = sp->GetTileID()};
    }

    // IAnimator
    if (auto* an = GetComponent<IAnimator>())
    {
      state.m_anData = {
        .m_key = an->GetKeyAnimation(),
        .m_keyAtlas = an->GetKeyAtlas(),
        .m_frames = an->GetFrames(),
        .m_step = an->GetStep(),
        .m_speed = an->GetSpeed(),
        .m_scale = an->GetScale()};
    }

    // IColor
    if (auto* c = GetComponent<IColor>())
    {
      auto &col = c->GetColor();
      state.m_cData = {col.m_r, col.m_g, col.m_b, col.m_a};
    }

    // IBoundingBox
    if(auto* bb = GetComponent<IBoundingBox>())
    {
      state.m_bbData = {
        .m_width     = bb->GetSize().x,
        .m_height    = bb->GetSize().y,
        .m_isTrigger = bb->GetTrigger(),
        .m_offset    = bb->GetOffset()};
    }

    // IText (the font is stored by key; empty if the text has no font yet)
    if(auto* tx = GetComponent<IText>())
    {
      const Font* font = tx->GetFont();
      state.m_txData = {tx->GetText(), tx->GetFontSize(), font ? font->GetName() : std::string{}};
    }

    // ICircleBox
    if(auto* cc = GetComponent<ICircleBox>())
      state.m_ccData = ICircleBoxData{.m_radius = cc->GetRadius(), .m_isTrigger = cc->IsTrigger()};

    // IMaterial (the shader is stored by key)
    if(auto* mt = GetComponent<IMaterial>())
      state.m_mtData = IMaterialData{.m_shaderKey = mt->GetKey()};

    // Game components registered in ComponentSerializers
    for (const ComponentSerializer& serializer : ComponentSerializers::GetAll())
    {
      nlohmann::json j;
      if (serializer.m_save(*this, j))
        state.m_components[serializer.m_key] = std::move(j);
    }

#ifndef NDEBUG
    WarnUnsavedComponents();
#endif

    return state;
  }

  void Object::Load(const ObjectState& state)
  {
    this->m_name  = state.m_name;
    this->m_layer = state.m_layer;
    this->m_renderSpace = state.m_renderSpace == static_cast<uint8_t>(RenderSpace::Screen)
                            ? RenderSpace::Screen : RenderSpace::World;
    this->m_visible = state.m_visible;

    auto& t = GetTransform();
    t.m_position  = state.m_tData.m_position;
    t.m_velocity  = state.m_tData.m_velocity;
    t.m_direction = state.m_tData.m_direction;
    t.m_angle     = state.m_tData.m_angle;
    t.m_prev_position  = state.m_tData.m_prev_position;
    t.m_prev_direction = state.m_tData.m_prev_direction;

    if(state.m_spData)
    {
      const ISpriteData& sp = *state.m_spData;
      if(!sp.m_tileID.empty())
        AddComponent<ISprite>(sp.m_keyName, sp.m_tileID, sp.m_scale);
      else if(!sp.m_animKey.empty())
        AddComponent<ISprite>(sp.m_keyName, sp.m_animKey, 0, sp.m_scale);   // IAnimator sets the frame
      else
        AddComponent<ISprite>(sp.m_keyName, sp.m_scale);
    }

    if(state.m_anData)
    {
      AddComponent<IAnimator>(
          state.m_anData->m_keyAtlas,
          state.m_anData->m_key,
          state.m_anData->m_speed,
          state.m_anData->m_step,
          state.m_anData->m_scale
          );
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

    if(state.m_bbData)
    {
      auto& bb = AddComponent<IBoundingBox>(
          Vector2{state.m_bbData->m_width, state.m_bbData->m_height},
          state.m_bbData->m_isTrigger);
      bb.SetOffset(state.m_bbData->m_offset);
      bb.Update(t.m_position);
    }

    if(state.m_txData)
    {
      Font* f = nullptr;
      if(!state.m_txData->m_fontKey.empty())
        f = Services::Fonts().GetFont(state.m_txData->m_fontKey);
      auto& tx = AddComponent<IText>("");
      tx.SetText(state.m_txData->m_text);
      tx.SetFontSize(state.m_txData->m_fontSize);
      tx.SetFont(f);
    }

    if(state.m_ccData)
    {
      auto& cc = AddComponent<ICircleBox>(state.m_ccData->m_radius, state.m_ccData->m_isTrigger);
      cc.Update(t.m_position);
    }

    if(state.m_mtData && !state.m_mtData->m_shaderKey.empty())
      AddComponent<IMaterial>(state.m_mtData->m_shaderKey);

    // Game components: each key is handed to its registered serializer.
    for (const auto& [key, j] : state.m_components)
    {
      if (const ComponentSerializer* serializer = ComponentSerializers::Find(key))
        serializer->m_load(*this, j);
      else
        LOG_ERROR(" | << Object '" + m_name + "': no serializer registered for '" + key + "'");
    }
  }

#ifndef NDEBUG
  namespace
  {
    /// Readable name of a type ("APP::IFoo" instead of "N3APP4IFooE") where the ABI allows it.
    std::string TypeName(std::type_index type)
    {
#if defined(__GNUG__)
      int status = 0;
      std::unique_ptr<char, void (*)(void*)> name(
        abi::__cxa_demangle(type.name(), nullptr, nullptr, &status), std::free);
      if (status == 0 && name)
        return name.get();
#endif
      return type.name();
    }
  }

  void Object::WarnUnsavedComponents() const
  {
    // Components the engine saves by itself in Save() / Load().
    static const std::type_index natives[] = {
      typeid(ISprite), typeid(IAnimator), typeid(IColor), typeid(IBoundingBox),
      typeid(ICircleBox), typeid(IText), typeid(IMaterial)};
    // Warn once per type, not once per object.
    static std::unordered_set<std::type_index> warned;

    components.ForEachType([&](std::type_index type)
    {
      for (const std::type_index& native : natives)
        if (type == native)
          return;
      if (ComponentSerializers::IsRegistered(type) || ComponentSerializers::IsIgnored(type))
        return;
      if (warned.insert(type).second)
        LOG_ERROR(" | << Object '" + m_name + "': component '" + TypeName(type) +
                  "' is not saved (use ComponentSerializers::Register<T>() or Ignore<T>())");
    });
  }
#endif
}
