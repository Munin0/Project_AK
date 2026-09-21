// | -------------------------------
#include "Button.hpp"
// | -------------------------------
#include "Engine/Component/Component.hpp"
#include "Engine/Object/Object.hpp"
#include "Engine/Render/Batching/RAPIBatch.hpp"
#include "Engine/Render/Batching/RBatch.hpp"
#include "Engine/Render/Color/RColor.hpp"
#include "Engine/Render/Geometry/RGeometry.hpp"
#include "Engine/Render/Image/AtlasData.hpp"
#include "Engine/Services/Services.hpp"
#include "Engine/Text/Font/Font.hpp"
#include "Engine/Text/TextAPI.hpp"
#include "Engine/Utils/Log.hpp"
#include "Engine/Utils/RawGeometry.hpp"
#include "Engine/Utils/Vector2.hpp"
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
    m_box = &AddBoundingBox(dimention);
    stats.m_hp = 1;
    stats.m_hp_max = 0;
    stats.m_def = 0;
    stats.m_agi = 0;
    stats.m_str = 0;
  }

  Button::Button(const std::string& _name, const Vector2& dimention, std::function<int(std::any)> _func)
    : Object{_name}, m_callback{_func}, m_text{""}, m_fontColor{Color::Black}, m_bgColor{Color::White}, m_outlineColor{Color::Black}
  { 
    m_box = &AddBoundingBox(dimention);
  }
  
  void Button::Update(float dt)
  {
    m_box = GetSolidBox();
  }

  void Button::Draw(Batcher& b) const
  {

    if(m_sprite != nullptr)
    {
      b.DrawAtlasSprite(transform.m_position, m_box->GetSize(), m_sprite->atlasLayer, m_textureRect.uvMin, m_textureRect.uvMax,{1,1,1,1},0.0f);
    }
    else
    {
      Rectangle rect = {transform.m_position.x, transform.m_position.y,m_box->GetSize().x,m_box->GetSize().y};
      Drawer::DrawRectangle(rect,m_bgColor.GetColor());
      Drawer::DrawRectangleOutline(rect,m_outlineColor.GetColor());
    }

    if(!m_text.GetText().empty() && m_text.GetFont() != nullptr)
    {
      auto size = this->m_box->GetSize();
      auto textSize = TextAPI::Get().GetMeasureTextEx(this->m_text.GetFont(),this->m_text.GetText(), this->m_text.GetFontSize());
      Vector2 fontPosition = {
        (this->transform.m_position.x + size.x / 2.0f) - (textSize.x / 2.0f),
        (this->transform.m_position.y + size.y / 2.0f) + (textSize.y / 4.0f)
      };
      TextAPI::Get().DrawTextEx(*m_text.GetFont(), fontPosition, this->m_text.GetText(), this->m_text.GetFontSize(), this->m_fontColor.GetColor());
    }
  }

  void Button::Action(std::any _)
  {
    if(m_callback == nullptr)
      return;
    this->m_callback(_);
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
    const Vector2 size = GetSolidBox()->GetSize();
    const Vector2 pos = GetTransform().m_position;

    return {pos.x,pos.y,size.x,size.y};
  }
}
