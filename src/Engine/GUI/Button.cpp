// | -------------------------------
#include "Button.hpp"
// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Inputs/Mouse.hpp"
#include "Engine/Inputs/PollEvent.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Physics/Collision.hpp"
#include "Engine/Render/Batching/Drawer.hpp"
#include "Engine/Render/Batching/Batch.hpp"
#include "Engine/Render/Color/Color.hpp"
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Text/TextAPI.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/RawGeometry.hpp"
#include "Engine/Utils/Vector2.hpp"
#include "SDL3/SDL_mouse.h"
// | -------------------------------
#include <any>
#include <functional>
#include <string>
#include <utility>
// | -------------------------------

namespace ENG
{
  Button::Button(const std::string& _name, const Vector2& dimention)
    : Object{_name}, m_callback{nullptr}, m_text{""}, m_fontColor{Color::Black}, m_bgColor{Color::White}, m_outlineColor{Color::Black}
  {
    m_box = &AddComponent<IBoundingBox>(dimention);
    this->m_alive = true;
  }

  Button::Button(const std::string& _name, const Vector2& dimention, std::function<int(std::any)> _func)
    : Object{_name}, m_callback{_func}, m_text{""}, m_fontColor{Color::Black}, m_bgColor{Color::White}, m_outlineColor{Color::Black}
  { 
    m_box = &AddComponent<IBoundingBox>(dimention);
  }
  
  void Button::Update(float dt)
  {
    (void)dt;
  }

  void Button::OnPointer(const Vector2& mouse)
  {
    m_hover = CollisionPointRect(mouse, GetRect());
    // Check the hit first, then take the click: only the button under the mouse eats it.
    if(m_hover && PollEvent::Get().ConsumeMouseButtonPress(SDL_BUTTON_LEFT))
      Action();
  }

  void Button::Draw(Batcher& b) const
  {
    if(m_sprite != nullptr)
    {
      b.DrawAtlasSprite(
          m_transform.m_position,
          m_box->GetSize(),
          m_sprite->m_atlasLayer,
          m_textureRect.m_uvMin, m_textureRect.m_uvMax,
          {1,1,1,1},
          0.0f
      );
    }
    else
    {
      Rectangle rect = {m_transform.m_position.x, m_transform.m_position.y,m_box->GetSize().x,m_box->GetSize().y};
      Drawer::DrawRectangle(rect,m_bgColor.GetColor());
      Drawer::DrawRectangleOutline(rect,m_outlineColor.GetColor());
    }

    if(!m_text.GetText().empty() && m_text.GetFont() != nullptr)
    {
      auto size = this->m_box->GetSize();
      auto textSize = TextAPI::Get().GetMeasureTextEx(this->m_text.GetFont(),this->m_text.GetText(), this->m_text.GetFontSize());
      Vector2 fontPosition = {
        (this->m_transform.m_position.x + size.x / 2.0f) - (textSize.x / 2.0f),
        (this->m_transform.m_position.y + size.y / 2.0f) + (textSize.y / 4.0f)
      };
      TextAPI::Get().DrawTextEx(*m_text.GetFont(), fontPosition, this->m_text.GetText(), this->m_text.GetFontSize(), this->m_fontColor.GetColor());
    }
  }

  void Button::Action()
  {
    if(m_callback == nullptr)
      return;
    this->m_callback(this->m_data);
  }

  void Button::SetBackgroundColor(const Color& color)
  {
    this->m_bgColor.ChangeColor(color);
  }

  void Button::SetOutlineColor(const Color& color)
  {
    this->m_outlineColor.ChangeColor(color);
  }

  void Button::SetTexture(const std::string& atlas, const std::string& textName)
  {
    m_sprite = Services::Assets().GetAtlas(atlas);
    if(m_sprite != nullptr)
    {
      m_textureRect = GetTileUV(*m_sprite,textName);
    }
  }

  void Button::SetFunction(ButtonCallback func)
  {
    if(func == nullptr)
    {
      LOG_ERROR(" | << Function not asigned to " + this->GetName());
      this->m_callback = nullptr;
    }
    this->m_callback = func;
  }

  void Button::SetFont(Font& font)
  {
    this->m_text.SetFont(&font);
  }

  void Button::SetData(std::any _)
  {
    this->m_data = std::move(_);
  }
  
  void Button::SetText(const std::string& text)
  {
    this->m_text.SetText(text);
  }

  void Button::SetTextSize(float size)
  {
    this->m_text.SetFontSize(size);
  }

  void Button::SetTextColor(const Color& color)
  {
    this->m_fontColor = color;
  }

  std::any& Button::GetData(void)
  {
    return this->m_data;
  }

  const std::any& Button::GetData(void) const
  {
    return this->m_data;
  }

  Rectangle Button::GetRect(void)
  {
    const Vector2 size = GetComponent<IBoundingBox>()->GetSize();
    const Vector2 pos = GetTransform().m_position;

    return {pos.x,pos.y,size.x,size.y};
  }
}
